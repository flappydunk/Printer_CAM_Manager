static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <title>PRINTER CAM</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="
            https://cdn.jsdelivr.net/npm/chart.js">
    </script>

<style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;background-color: #f1f1f1;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #2f4468;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
      img {  width: auto ;
        max-width: 100% ;
        height: auto ; 
      }

/* Style the tab */
.tab {
  overflow: hidden;
  border: 1px solid #ccc;
  background-color: #f1f1f1;
}

/* Style the buttons inside the tab */
.tab button {
  background-color: inherit;
  float: center;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 14px 16px;
  transition: 0.3s;
  font-size: 17px;
}

/* Change background color of buttons on hover */
.tab button:hover {
  background-color: #ddd;
}

/* Create an active/current tablink class */
.tab button.active {
  background-color: #ccc;
}

/* Style the tab content */
.tabcontent {
  display: none;
  padding: 6px 12px;
  border: 1px solid #ccc;
  border-top: none;
}

.green {background-color: #04AA6D;} /* Green */
.blue {background-color: #008CBA;} /* Blue */
.red {background-color: #f44336;} /* Red */
.gray {background-color: #e7e7e7; color: black;} /* Gray */
.black {background-color: #555555;} /* Black */


.button {
  border: none;
  border-radius: 20px;
  color: white;
  padding: 8px 10px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 14px;
}


input[type='number']{
    width: 50px;
} 

table { 
    margin-left: auto;
    margin-right: auto;
    text-align: center;
}

.chart-container {
  width: 500px;
  height:1000px;
  background-color: #f1f1f1;
}


</style>
</head>


  <body>
<h2>ESP Printer Monitor</h2>
<table> 
  <tr>
    <td align="center"><img src="" id="photo" style="border:10px solid #f44336;"></td>
  </tr>
  <tr>
    <td align="center"><div id="file-label"></div></td>
  </tr>
</table>
<table>
  <tr>
    <td align="center"><button class="button" id="light-btn" onclick="Send('light');">Light</button></td>
    <td align="center"><button class="button" id="abort-btn" onclick="Send('Pause');">Pause Print</button></td>
  </tr>
  <tr>
    <td align="center"><div class="proglabel" id="prog-label">Progress: Reading... </div></td>
    <td align="center"><div class="elapsedtime" id="elapsed-time">Elapsed time: Reading...</div></td>
    
  </tr>
</table>



<div class="tab">
  <button class="tablinks" onclick="openTab(event, 'Temps')">Temps</button>
  <button class="tablinks" onclick="openTab(event, 'Actions')">Actions</button>
  <button class="tablinks" onclick="openTab(event, 'Commands')">Commands</button>
  <button class="tablinks" onclick="openTab(event, 'SD_Card')">SD Card</button>
</div>

<div id="Temps" class="tabcontent">
    <table style="width:100%" >
        <tr>
          <td align="center">
            <div class="chart-container">
              <canvas id="myChart" width="200" height="200" style="border:1px solid"></canvas>
            </div>
          </td>        
        </tr>
   </table>
</div>

<div id="Actions" class="tabcontent">  
      <table style="width:100%" >
        <tr>
         <td>
        	<h2>Heating</h2>
          	<label for="Extr_temp">Heat:</label>
          	<button class="button green" type="button" class="button" onclick="Send('Extr_temp_off');">Off</button>
          	<select id="Extr_temp" name="Extr_temp">
             		<option value="0">0.0 (Off)</option>
             		<option value="185">185.0 (PLA)</option>
             		<option value="230">230.0 (ABS)</option>
          	</select>
              	<button class="button red" type="button"  class="button" onclick="Send('Extr_temp');">Set</button>
      	</td>
	<td>
		<br>
		<br>
        	<h2>Filament</h2>
        	<button type="button" class="button blue" class="button" onclick="Send('Fil_For');">Extrude</button>
        	<button type="button" class="button blue" class="button" onclick="Send('Fil_Bak');">Reverse</button><br>
      
        	<label for="Fil_mm">Length mm </label>
        	<input type="number" id="Fil_mm" name="Fil_mm" min="0" value="10"><br>
        	<label for="Fil_sp">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Speed</label>
        	<input type="number"  id="Fil_sp" name="Fil_sp" value="100">
        	<br>
      
          </td>
	</tr>

	<tr>
      	  <td>
          	<label for="Bed">Bed:&nbsp</label>
          	<button class="button green" type="button" class="button" onclick="Send('Bed_off');">Off</button>
          	<select id="Bed_on" name="Bed_on">
             	<option value="0">0.0 (Off)</option>
             	<option value="60">60.0 (PLA)</option>
             	<option value="110">110.0 (ABS)</option>
          	</select>
          	<button class="button red" type="button" class="button" onclick="Send('Bed_on');">Set</button><br>
      
          </td>
	  <td>
	  </td>  
        </tr>
        
        <tr>
        <td>
        	<h2>Homing</h2>
          	<label for="Home">Home axis:</label>		
          	<button type="button" class="button blue" class="button" onclick="Send('Home');">Home</button>
		<br>
          	<select id="Home" name="Home">
            	<option value="X">X</option>
            	<option value="Y">Y</option>
            	<option value="Z">Z</option>
            	<option value="All">All</option>
          	</select>
        </td>
	<td>       	
		<h2>Move Axis</h2>
          	<label>Move axis:</label>
          	<button type="button" class="button blue" class="button" onclick="Send('Axis_m');">Move</button>
		<br>
          	<select id="Axis_m" name="Axis_m">
             		<option value="X">X</option>
             		<option value="Y">Y</option>
             		<option value="Z">Z</option>
          	</select>
        	<select id="Axis_d" name="Axis_d">
             		<option value="-">-</option>
             		<option value="+">+</option>
          	</select>
      
        	<select id="Axis_l" name="Axis_l">
             		<option value="0.1">0.1</option>
             		<option value="1">1</option>
             		<option value="10">10</option>
          	</select> 
          </td>

         
        </tr>
       
       
      </table>

    <button class="button" id="power-btn" onclick="Send('CAM_Restart');">CAMERA Restart</button>
    <button class="button" id="power-btn" onclick="Send('PTR_Reset');">PRINTER Reset</button>
     

</div>  

<div id="Commands" class="tabcontent">
    <table style="width:100%" >
        <tr>
          	<td align="center">
			<h2>Commands</h2>
      
        		<button type="button" class="button blue" class="button" onclick="Send('pFan');">Print Fan</button>
        		<button type="button" class="button blue" class="button" onclick="Send('light');">Light</button>
        		<button type="button" class="button blue" class="button" onclick="Send('Pause');">Pause</button>
		</td> 
        </tr>  
	<tr>
        	<td>
        		<label>Type Command </label>
        		<input type="text" id="cmd" name="cmd" min="0" >
        		<button type="button" class="button blue" class="button" onclick="Send('cmd');">Send</button> <br>
        		
        		
            
          	</td>        
        </tr>
	<tr>
		<td>
			<button type="button" id="logbutton" class="button blue" class="button" onclick="Send('Log');">Log</button> 
		</td>
	</tr>
	<tr>
		<td>
			<textarea readonly rows='10'  cols='60' maxlength="1000" style="resize: none;" id='log' name = 'log' ></textarea> <br/>
		</td>
	</tr>

   </table>
</div>
 

<div id="SD_Card" class="tabcontent">
    <table>
        <tr>
          <h3>SD Card Listing</h3>
        </tr>
     </table>
     <table>
        <tr> 
          <td align="center"><button class="button" id="SD" onclick="Send('SD_List');">Refresh SD List</button></td>
          <td align="center"><button class="button" id="PrintList" onclick="Send('Print');">Print File</button></td> 
          <td align="center"><select id="Print" name="Print" size="1" style="width: 150px;" ></select></td>  
        </tr>
     </table>
     <table>    
         <tr align="center"><textarea rows='10'  cols='60'   id='SDCard' name = 'SDCard' >SD Card</textarea></tr>              
     </table> 
</div>

        
<script>
//Graph
// Define the X & Y Axis Scales
        const scales = {
            x: {
                title: {
                    display: true,
                    text: 'Duration'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Temperature'
                }
            }
        };
// Define some plugins to customize the chart
        const plugins = {
            // Customize the title of the chart
            title: {
                display: true,
                text: 'Printer Temperatures',
                color: '#8CBA51',
                font: {
                    size: '36'
                }
            }
        };


   
// Get the 2D rendering context of the canvas
        let ctx = document
        .getElementById('myChart')
        .getContext('2d');

        // Create a new Pie Chart
        let myChart = new Chart(ctx, {
        // Specify the chart type
        type: 'line',
            // Provide data for the chart
            data: {
                // Labels for x scale
                labels: [],
                // Datasets for the chart
                datasets: [{
                    label: 'E1',
                    data: [],
                    // Data points for each segment
                    backgroundColor: ['rgba(255, 99, 132, 0.8)'],
                    borderWidth: 2 // Border width for each segment
            },
            {
                    label: 'E1_set',
                    data: [],
                    // Data points for each segment
                    backgroundColor: ['rgba(255, 99, 132, 0.8)'],
                    borderWidth: 2 // Border width for each segment
            },
            {
                    label: 'Bed',
                    data: [],
                    // Data points for each segment
                    backgroundColor: ['rgba(54, 162, 235, 0.8)'],
                    borderWidth: 2 // Border width for each segment
            },
            {
                    label: 'Bed_set',
                    data: [],
                    // Data points for each segment
                    backgroundColor: ['rgba(54, 162, 235, 0.8)'],
                    borderWidth: 2 // Border width for each segment
                }]
            },
            // Additional options for the chart
            options: {
                responsive: true, //It make the chart responsive
                plugins: plugins,
                scales: scales
            }
        });


    function openTab(evt, tabName) {
      var i, tabcontent, tablinks;
      tabcontent = document.getElementsByClassName("tabcontent");
      for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
      }
      tablinks = document.getElementsByClassName("tablinks");
      for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
      }
      document.getElementById(tabName).style.display = "block";
      evt.currentTarget.className += " active";
    }
    
    window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";

    function seconds2time (seconds) {
        var hours   = Math.floor(seconds / 3600);
        var minutes = Math.floor((seconds - (hours * 3600)) / 60);
        var seconds = seconds - (hours * 3600) - (minutes * 60);
        var time = "";
    
        if (hours != 0) {
          time = hours+":";
        }
        if (minutes != 0 || time !== "") {
          minutes = (minutes < 10 && time !== "") ? "0"+minutes : String(minutes);
          time += minutes+":";
        }
        if (time === "") {
          time = seconds+"s";
        }
        else {
          time += (seconds < 10) ? "0"+seconds : String(seconds);
        }
        return time;
    }
 

    var Data_points = 0;
    var SDListActive = 0;
    var SD_List_length = 0;
    
    function Update(x) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var Data1 = this.responseText;

          if (Data1 != "") {
              var pValues = new Array();
              pValues = Data1.split(",");
              
              if (pValues[0] == "L"){
                var Loglabel = document.getElementById("log");
                Loglabel.innerHTML += pValues[1];
                Loglabel.scrollTop = Loglabel.scrollHeight;
              }
              if (pValues[0] == "F"){
                var filelabel = document.getElementById("file-label");
                filelabel.innerHTML = pValues[1];
              }
              if (pValues[0] == "P"){
                var proglabel = document.getElementById("prog-label");
                proglabel.innerHTML = pValues[1];
              }
              if (pValues[0] == "E"){
                var elapsetime = document.getElementById("elapsed-time");
                elapsetime.innerHTML = pValues[1];
              }
              if (pValues[0] == "T"){
                //Update the chart object
                let t1_value = parseFloat(pValues[1]);
                let t2_value = parseFloat(pValues[2]);
                let t3_value = parseFloat(pValues[3]);
                let t4_value = parseFloat(pValues[4]);
              
                let new_tsecs = pValues[5];
                let updateTime = seconds2time(new_tsecs);
                
                
                if (Data_points > 20){
                  //remove old data point
                  myChart.data.labels.shift();
                  myChart.data.datasets[0].data.shift();
                  myChart.data.datasets[1].data.shift();
                  myChart.data.datasets[2].data.shift();
                  myChart.data.datasets[3].data.shift();
                }
                
                //update segment
                myChart.data.labels.push(updateTime);
                myChart.data.datasets[0].data.push(t1_value);
                myChart.data.datasets[1].data.push(t2_value);
                myChart.data.datasets[2].data.push(t3_value);
                myChart.data.datasets[3].data.push(t4_value);
  
                Data_points++;
                myChart.update();
              }

              //The rest of the array is for the SD Card first field [0] = "S"
                if (pValues[0] == "S"){
                  //drop down select box
                  
                  
                  //remove old values
                  var x = document.getElementById('Print');
                  var d = x.length;
                  
                  while (d > -1){
                    x.remove(d);
                    d--;
                  }

                  //3 fields per file
                  SD_List_length = 0;
                  for (i = 1; i < pValues.length; i=i+3){                                
                    document.getElementById('Print').innerHTML += '<option value=' + pValues[i] + '>' + pValues[i] + '</option>';
                    SD_List_length++;
                  }
                   
                  //text area
                  document.getElementById("SDCard").value = "";
                  
                  //text positioned at 0, 15, 32
                  for (i = 1; i < pValues.length - 1; i=i+3) {  
                   while(pValues[i].length < 14){
                    pValues[i] = pValues[i] + " ";
                   }
                   while(pValues[i+1].length < 16){
                    pValues[i+1] = pValues[i+1] + " ";
                   }
                   
                   let f1 = pValues[i] + pValues[i+1]  +  pValues[i+2];
                   document.getElementById("SDCard").value +=  f1 + "\n";         

                }           
              }
          }
          
        }
      };
      xhttp.open("GET", "/action?go=" + x, true);
      xhttp.send();
    }


   //for buttons
   var logging = 0;
   function Send(x) {
     var xhr = new XMLHttpRequest();

     if (x == 'Extr_temp_off'){xhr.open("GET", "/action?go=" + x, true);};
     if (x == 'Extr_temp'){ 
      var tempE = document.getElementById(x).value;
      xhr.open("GET", "/action?go=Extr_temp" +","+ tempE + ",", true);
     };

     if (x == 'Bed_off'){xhr.open("GET", "/action?go=" + x, true);};
     if (x == 'Bed_on'){ 
      var tempB = document.getElementById(x).value;
      xhr.open("GET", "/action?go=Bed_on" +","+ tempB + ",", true);
     };
     if (x == 'Fil_For'){ 
      var Fmm = document.getElementById('Fil_mm').value;
      var Fsp = document.getElementById('Fil_sp').value;
      xhr.open("GET", "/action?go=Fil_For" +","+ Fmm + "," + Fsp + ",", true);
     };
     if (x == 'Fil_Bak'){ 
      var Fmm = document.getElementById('Fil_mm').value;
      var Fsp = document.getElementById('Fil_sp').value;
      xhr.open("GET", "/action?go=Fil_Bak" +","+ Fmm + "," + Fsp + ",", true);
     };

     if (x == 'Home'){ 
      var Hm = document.getElementById('Home').value;
      xhr.open("GET", "/action?go=Home" +","+ Hm + ",", true);
     };
     if (x == 'Axis_m'){ 
      var am = document.getElementById(x).value;
      var ad = document.getElementById('Axis_d').value;
      var al = document.getElementById('Axis_l').value;
      xhr.open("GET", "/action?go=Axis_m" +","+ am + "," + ad + "," + al+ ",", true);
     };
     if (x == 'pFan'){  xhr.open("GET", "/action?go=" + x, true);};
     if (x == 'light'){  xhr.open("GET", "/action?go=" + x, true);};

     if (x == 'cmd'){ 
      var cm = document.getElementById(x).value;
      document.getElementById(x).value = "";
      xhr.open("GET", "/action?go=cmd" +","+ cm + ",", true);
     };
     if (x == 'Pause'){  
      if (confirm("Confirm you want to continue?")) {
          xhr.open("GET", "/action?go=" + x, true);
      }      
     }
     if (x == 'CAM_Restart'){  
      if (confirm("Confirm you want to continue?")) {
          xhr.open("GET", "/action?go=" + x, true);
      }      
     }

     if (x == 'PTR_Reset'){  
      if (confirm("Confirm you want to continue?")) {
          xhr.open("GET", "/action?go=" + x, true);
      }      
     }
     if (x == 'SD_List'){  
        if (confirm("Confirm you want to continue? \n ")) {
            document.getElementById('SDCard').value = "";
            //remove old values        
            var p = document.getElementById('Print');
            var l = p.length;  
            while (l > -1){
              p.remove(l);
              l--;
            }

            xhr.open("GET", "/action?go=" + x, true);
        }
     } 
      if (x == 'Print'){ 
        var e = document.getElementById(x); 
        var value = e.value;
        var text = e.options[e.selectedIndex].value;
        if (confirm("Are you sure you want to start print of file?\n\n" + value)) {
            xhr.open("GET", "/action?go=Print" + "," + value  + ",", true);
        } 
      
      }
      if (x == 'Log'){
        if (logging == 0) {
          logging = 1;
          x += '_on';
          document.getElementById("logbutton").style.background = 'red';
        }else{
          logging = 0;
          x += '_off';
          document.getElementById("logbutton").style.background = '#008CBA';
        }
        xhr.open("GET", "/action?go=" + x, true);
      }
     xhr.send();
   }


   setInterval(function() {
      // Call a function at interval to update Graph
      Update('values');
    }, 2000); //2000mSeconds update rate

    </script>
  </body>
</html>
)rawliteral";
