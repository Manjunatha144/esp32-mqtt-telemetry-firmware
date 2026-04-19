TESTING NOTES



Test 1: Telemetry visible

\- Steps:

&nbsp; 1) Run: python simulator\\publisher.py

&nbsp; 2) Open MQTT Explorer or Node-RED Debug/Dashboard

\- Expected: telemetry messages appear on edgepulse/manjunatha144/device\_001/telemetry

\- Result: PASS/FAIL

\- Notes:



Test 2: Node-RED dashboard

\- Expected: temp chart updates; SEQ increases; Uptime updates

\- Result: PASS/FAIL

\- Notes:



Test 3: CMD interval change

\- Steps:

&nbsp; 1) Click Node-RED button "Set 1s" (publishes {"interval\_ms":1000} to cmd topic)

\- Expected: publisher rate becomes faster; terminal shows CMD interval changed

\- Result: PASS/FAIL

\- Notes:



Test 4: OFFLINE/ONLINE last-seen

\- Steps:

&nbsp; 1) Stop publisher (Ctrl+C) for >15 seconds

&nbsp; 2) Start publisher again

\- Expected: Status becomes OFFLINE then ONLINE

\- Result: PASS/FAIL

\- Notes:



Test 5: CSV logger

\- Steps:

&nbsp; 1) Run: python simulator\\logger.py for 1–2 minutes

\- Expected: simulator\\telemetry.csv created; new rows added

\- Result: PASS/FAIL

\- Notes:

