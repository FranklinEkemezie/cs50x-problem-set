
SELECT
    p1.name as call_caller,
    p2.name as call_receiver,
    p1.passport_number as caller_pn,
    p2.passport_number as receiver_pn,
    flights.id as flight_id,
    phone_calls.duration as call_duration
FROM phone_calls

-- Join to get the caller
JOIN people p1
    ON p1.phone_number = phone_calls.caller

    -- The caller is the thief 'Bruce'
    AND p1.name = 'Bruce'

-- Join once more to get the receiver
JOIN people p2
    ON p2.phone_number = phone_calls.receiver


-- Get the flights taken early in the morning
JOIN flights

    ON flights.day     = 29
    AND flights.month   = 7
    AND flights.year    = 2023
    AND flights.hour    < 12


-- Get the passengers who took that flight
JOIN passengers
        ON passengers.flight_id = flights.id

        -- and, received a call on the day the crime took place
        AND phone_calls.day     = 28
        AND phone_calls.month   = 7
        AND phone_calls.year    = 2023

-- Remove duplication (OPTIONAL)
GROUP BY call_caller, call_receiver

-- Thief spoke with accomplise within a very short period time
ORDER BY phone_calls.duration
LIMIT 1;
