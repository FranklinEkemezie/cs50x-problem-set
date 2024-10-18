-- SQL query to find the city the thief escaped to:

-- This query assumes the thief is known to be 'Bruce' and
-- only one person

SELECT * FROM flights
JOIN airports 
    -- Join only if the airport was a destination
    ON airports.id = flights.destination_airport_id

WHERE flights.id IN (

    -- Get the ID of the flight the thief took
    SELECT flight_id FROM passengers
    WHERE passport_number IN (

        SELECT passport_number FROM people
        WHERE name = 'Bruce'
    )
)

-- Flight must have taken place the next day after the crime,
-- and some time early in the morning
AND day = 29 AND month = 7 AND year = 2023  AND hour < 12
