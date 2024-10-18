
SELECT 
    p2.name as call_receiver
FROM phone_calls

-- Join to get the caller
JOIN people p1
    ON p1.phone_number = phone_calls.caller

    -- The caller is the thief 'Bruce'
    AND p1.name = 'Bruce'

-- Join once more to get the receiver
JOIN people p2
    ON p2.phone_number = phone_calls.receiver

-- Get the passengers who received a call on the day of the crime
JOIN passengers
    ON p2.passport_number = passengers.passport_number

    -- and, received a call on the day the crime took place
    AND phone_calls.day       = 28
    AND phone_calls.month   = 7
    AND phone_calls.year    = 2023 

-- Get the flights taken by the passengers
JOIN flights
    ON flights.id = passengers.flight_id

-- Thief spoke with accomplise within a very short period time
ORDER BY phone_calls.duration
LIMIT 1