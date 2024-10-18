-- SQL query to find who the thief is:


-- In the beginning, I only know that the theft took place on July 28, 2023
-- and that the theft took place on Humphrey Street.
-- First of all, get the description of crimes that took place on that
-- particular day, and that street:

-- The description of crime scene report with ID 295 describes a
-- similar crime scene. The theft took place at 10:15, interviews were
-- conducted at the time, and the interview transcripts mentioned
-- something about 'bakery'


-- In the interview transcripts, three people said something about 'bakery':
-- - Ruth mentioned that the thief got into a car parked in the parking lot
--      and left within ten minutes of the theft;
-- - Eugene said that he recognized the thief withdrawing money from an ATM
--      along Leggett Street earlier that monring;
-- - Raymond said he saw the thief make a brief call to someone while leaving
--      the bakery asking the accomplice to purchase a flight ticket to take
--      the earliest flight out of Fiftyville the next day

-- SEE log.sql for more info


-- Get the name of the thief

SELECT name FROM people

WHERE

-- From Ruth's interview transcript
license_plate IN (

    -- Get people who left the bakery parking lot around 10 minutes
    -- after the crime has taken place
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND year = 2023
    AND hour = 10 AND minute BETWEEN 15 AND 30
    and activity = 'exit'
)

-- From Eugene's interview transcript
AND id IN (

    -- Get the ID of people who withdrew money from ATM
    -- located in Leggett Street earlier that day
    SELECT bank_accounts.person_id FROM atm_transactions
    JOIN bank_accounts
        ON bank_accounts.account_number = atm_transactions.account_number
    WHERE   atm_transactions.day            = 28    AND
            atm_transactions.month          = 7     AND
            atm_transactions.year           = 2023  AND
            atm_transactions.atm_location   = 'Leggett Street'
)

-- From Raymond's interview transcript
AND phone_number IN (

    -- Get people who made calls on that day, and
    -- also took a flight the next day
    SELECT caller FROM phone_calls
    WHERE day = 28 AND month = 7 AND year = 2023
    AND caller IN (

        -- Get phone number of people who took a flight
        -- the next day early in the morning
        SELECT phone_number FROM people
        WHERE passport_number IN (

            -- Get passengers whose flight left early in the 
            -- morning, the day after the crime occurred
            SELECT passport_number FROM passengers
            WHERE flight_id IN (

                -- Get the flights taken the next day,
                -- leaving Fiftyville early in the monring
                SELECT flights.id as flight_id FROM flights
                JOIN airports
                    ON airports.id      = flights.origin_airport_id
                    AND airports.city   = 'Fiftyville'
                WHERE   flights.day     = 29    AND
                        flights.month   = 7     AND
                        flights.year    = 2023  AND
                        flights.hour    < 12
            )
        )
    )
)

