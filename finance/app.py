import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user's current cash balance
    user_id = session["user_id"]
    rows = db.execute(
        "SELECT * FROM users WHERE id = ?",
        user_id
    )
    if len(rows) != 1:
        return apology("user not found", 403)
    cash = float(rows[0]["cash"])
    username = rows[0]["username"]

    # Get the stocks owned by the user
    transaction_rows = db.execute(
        "SELECT * FROM transactions WHERE user_id = ?",
        user_id
    )
    
    # Compute the stocks
    stocks = []

    # Keep track of symbols added in the stocks list
    symbols_in_stocks = []
    for row in transaction_rows:

        symbol  = row["symbol"].upper()
        shares  = row["shares"]
        price   = lookup(symbol)["price"]
        total   = shares * price

        if symbol not in symbols_in_stocks:
            stock = {
                "symbol":   symbol,
                "shares":   shares,
                "price":    price,
                "total":    total
            }

            stocks.append(stock)
            symbols_in_stocks.append(symbol)

        else:
            stock_index = next(
                i for i, stock in enumerate(stocks) if stock["symbol"] == symbol.upper()
            )

            # Update the stocks based on 'buy' or 'sell'
            if row["type"] == "buy":
                stocks[stock_index]["shares"]   += shares
                stocks[stock_index]["total"]    += total
            elif row["type"] == "sell":
                stocks[stock_index]["shares"]   -= shares
                stocks[stock_index]["total"]    -= total

    # Calculate the grand total
    total = sum(
        [stock["total"] for stock in stocks]
    ) + cash

    return render_template("index.html", stocks=stocks, cash=cash, total=total, username=username)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Check for the symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol", 403)
        
        # Check if the symbol could be found
        quote_data = lookup(symbol)
        if quote_data is None:
            return apology(f"symbol '{symbol}' is not found", 403)
        symbol = symbol.upper()
        
        # Ensure the amount of shares to buy was submitted
        shares = request.form.get("shares")
        if not shares:
            return apology("must provide number of shares")
        
        # Ensure the amount of shares is an integer (a positive one)
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("shares must be a positive integer")
        except ValueError:
            return apology("shares must be an integer")
        except:
            return apology("invalid number of shares")
        
        # Be sure the user has enough cash to buy shares
        user_id = session["user_id"]
        rows = db.execute(
            "SELECT cash FROM users WHERE  id = ?", user_id  
        )
        if len(rows) != 1:
            return apology("invalid user: cannot buy stocks", 403)
        

        user_cash   = float(rows[0]["cash"])
        price       = float(quote_data["price"])
        if user_cash < price:
            return apology("not enough cash to buy stocks")
        
        # Insert the 'buy' details to database
        query = """
        INSERT INTO transactions (user_id, type, symbol, shares, price)
        VALUES (?, 'buy', ?, ?, ?)
        """
        db.execute(query, user_id, symbol, shares, price)

        # And then, update the user's price
        current_cash = user_cash - (shares * price)
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", current_cash, user_id
        )

        flash("Stock bought successfully!")
        return redirect("/")
    
    return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get the transactions
    query = """
    SELECT * FROM transactions
    WHERE user_id = ?
    """

    rows = db.execute(query, session["user_id"])
    
    transaction_history = []
    for row in rows:
        transaction = {
            "symbol":   row["symbol"].upper(),
            "type":     row["type"].upper(),
            "price":    row["price"],
            "shares":   row["shares"],
            "datetime": row["datetime"]
        }

        transaction_history.append(transaction)

    return render_template("history.html", transaction_history=transaction_history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Check if request is by POST or by GET
    if request.method == "POST":
        # Ensure the symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol", 403)
        
        # Look up the stock's current price
        quote_data = lookup(symbol)
        if quote_data is None:
            return apology(f"Symbol '{symbol}' not found", 403)
        
        return render_template("quoted.html", quote_data=quote_data)
    
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Check if the request is by POST or by GET
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)
        
        # Ensure the password was submitted
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 403)
        
        # Ensure the confirmation password was submitted
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must provide confirmation password", 403)
        
        # Check if password matches confirmation password
        if confirmation != password:
            return apology(f"passwords: {password} != {confirmation} do not match", 403)
        
        # Try to insert name
        query = "INSERT INTO users (username, hash) VALUES (?, ?)"
        try:
            db.execute(query, username, generate_password_hash(password))
        except ValueError:
            return apology("user already registered")
        except:
            return apology("something went wrong")
        
        # Go to login page
        flash("Login successful")
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Ensure the user input a stock's symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide stock's symbol", 403)
        
        # Check if the stock is found, and get the price
        quote_data = lookup(symbol)
        if quote_data is None:
            return apology(f"symbol '{symbol}' is not found", 403)
        symbol = symbol.upper()

        # Ensure the amount of shares to buy is specified
        shares = request.form.get("shares")
        if not shares:
            return apology("must provide shares", 403)
        
        # Ensure the amount of shares is an integer (a positive one)
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("shares must be a positive integer")
        except ValueError:
            return apology("shares must be an integer")
        except:
            return apology("invalid number of shares")
        

        # Be sure the user exists
        user_id = session["user_id"]
        rows = db.execute(
            "SELECT * FROM users WHERE id = ?", user_id
        )
        if len(rows) != 1:
            return apology("invalid user: cannot sell stocks", 403)
        user_cash = rows[0]["cash"]

        # Be sure the user has enough of those shares of the symbol to sell
        query = """
        SELECT
            COALESCE(SUM(buys.shares), 0) AS shares_bought,
            COALESCE(SUM(sells.shares), 0) AS shares_sold
        FROM transactions tr

        LEFT JOIN transactions buys
        ON  buys.type = 'buy'
        AND buys.id !=  tr.id
        AND buys.symbol = :symbol

        LEFT JOIN transactions sells
        ON  sells.type = 'sell'
        AND sells.id != tr.id
        AND sells.symbol = :symbol

        WHERE tr.user_id = :user_id
        AND tr.symbol = :symbol
        """

        rows = db.execute(query, user_id=user_id, symbol=symbol)
        shares_bought   = rows[0]["shares_bought"]
        shares_sold     = rows[0]["shares_sold"]

        # Compute the shares left after buying, and then compare
        shares_left     = shares_bought - shares_sold - shares
        if shares_left < 0:
            return apology("user does not own enough amount stock shares to sell")
        
        # Go on to perform the transaction
        query = """
        INSERT INTO transactions (user_id, type, symbol, shares, price)
        VALUES (?, 'sell', ?, ?, ?)
        """

        price = quote_data["price"]
        db.execute(query, user_id, symbol, shares, price)

        # And then, update the user's price
        current_cash = user_cash + (shares * price)
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", current_cash, user_id
        )

        flash("Stock sold successfully!")
        return redirect("/")
        
    else:
        # Get the symbols of the stocks owned by the user
        user_id = session["user_id"]
        rows = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id
        )
        symbols = [row["symbol"] for row in rows]

        return render_template("sell.html", symbols=symbols)


@app.route("/topup", methods=["POST"])
@login_required
def addcash():
    """Add aditional cash to their account"""

    # Ensure the user provides the top up amount
    amount = request.form.get("amount")
    if not amount:
        return apology("must provide topup amount", 403)
    
    # Ensure the amount is valid
    try:
        amount = float(amount)
        if amount < 0:
            return apology("must provide a positive amount", 403)
        
    except ValueError:
        return apology("must provide a valid amount", 403)
    except:
        return apology("could not top up cash", 403)
    
    # Update their cash
    db.execute(
        "UPDATE users SET cash = cash + ? WHERE id = ?", amount, session["user_id"]
    )

    flash(f"Successfully added ${amount} to cash")
    
    return redirect("/")

if __name__ == "__main__":
    app.run(debug=True)
