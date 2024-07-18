const char* htmlConfigTZ = R"literal(
<!DOCTYPE html>
<html>
  <head><title>Set Time</title></head>
  <body>
    <h1>Set Server Time</h1>
    <h2>Server Time</h2>
    Local Time: %server_time%
    <h2>Client Time</h2>
    Local Time: <span id="clienttime">0:00:00 UTC</span>
    <form id="TZForm" method="POST" action=configTZ onsubmit="addTime()">
      <label for="timezone">Timezone:</label>
      <select id="timezone" name="timezone">
      %tz_options%
      </select><br>
      <p><button type="submit">Set Time</button>
    </form>
    <script>
       function addTime(){
         var lform = document.getElementById('TZForm');
         var utctime = document.createElement('input');
         utctime.type = 'hidden';
         utctime.name = 'utctime';
         utctime.value = Date.now();
         lform.appendChild(utctime);
         console.log("utctime: " + Date.now());
         lform.submit();
       }
       var tz = Intl.DateTimeFormat().resolvedOptions().timeZone;
       document.getElementById("clienttime").textContent = 
          Date().toLocaleString() + " " + tz;
       var tzSelect = document.getElementById("timezone");
       for(var i, j = 0; i = tzSelect.options[j]; j++) {
         if(i.text == tz) {
            tzSelect.selectedIndex = j;
            break;
          }
       }
    </script>
   </body>
</html>
)literal";
