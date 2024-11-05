--- Get the buys made by user with ID

SELECT
    COALESCE(SUM(shares), 0) AS shares_bought
FROM transactions
WHERE user_id = 1
AND symbol = 'MSFT'
AND type = 'buy';

SELECT '-----------------------------' FROM users LIMIT 1;


-- Get the sales made by user with ID

SELECT
    COALESCE(SUM(shares), 0) AS shares_sold
FROM transactions
WHERE user_id = 1
AND symbol = 'MSFT'
AND type = 'sell';

SELECT '-----------------------------' FROM users LIMIT 1;

-- Get the sales bought and sales sold by user with the given ID

SELECT
    tr.id,

    '  ',
    buys.id,
    buys.type,
    buys.symbol,
    buys.shares,
    buys.price,

    '  ',

    sells.id,
    sells.type,
    sells.symbol,
    sells.shares,
    sells.price

FROM transactions tr

LEFT JOIN transactions buys
ON buys.type = 'buy'
AND buys.id != tr.id
AND buys.symbol = 'MSFT'

LEFT JOIN transactions sells
ON sells.type = 'sell'
AND sells.id != tr.id
AND sells.symbol = 'MSFT'

WHERE tr.user_id = 1
AND tr.symbol = 'MSFT'

GROUP BY tr.id;



SELECT '-----------------------------' FROM users LIMIT 1;

SELECT
    COALESCE(SUM(buys.shares), 0) AS shares_bought,
    COALESCE(SUM(sells.shares), 0) AS shares_sold
FROM transactions tr

LEFT JOIN transactions buys
ON buys.type = 'buy'
AND buys.id != tr.id
AND buys.symbol = 'MSFT'

LEFT JOIN transactions sells
ON sells.type = 'sell'
AND sells.id != tr.id
AND sells.symbol = 'MSFT'

WHERE tr.user_id = 1
AND tr.symbol = 'MSFT'

GROUP BY tr.id;
