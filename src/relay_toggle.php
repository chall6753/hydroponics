<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="utf-8">
    <title>Controls</title>
    <style>
      html {
          font-family: Arial;
          display: inline-block;
          margin: 0px auto;
          text-align: center;
      }
      
      h1 { font-size: 2.5rem; color:black;}
      h2 { font-size: 1.25rem; color:#2980b9;}
      
      .onButton {
        display: inline-block;
        padding: 15px 25px;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        text-align: center;
        color: #fff;
        background-color: #4CAF50;
        border: 2px solid black;
        border-radius: 15px;
        margin: 5px;
        
      }
      .onButton:hover {background-color: #3e8e41}
      .onButton:active {
        background-color: #3e8e41;
        box-shadow: 0 1px #666;
        transform: translateY(4px);
      }
        
      .offButton {
        display: inline-block;
        padding: 15px 25px;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        text-align: center;
        color: #fff;
        background-color: #e74c3c;
        border: 2px solid black;
        border-radius: 15px;
        margin: 5px;
        
      }
      .offButton:hover {background-color: #c0392b}
      .offButton:active {
        background-color: #c0392b;
        transform: translateY(4px);
      }
    </style>
  </head>
  <body>
    <h1>Relay Controls and Status</h1>
    
    <form action="Update_Relay_DB.php" method="GET" id="pumpOn" >
        <input type="hidden" name="id" value="1"/>  
        <input type="hidden" name="status" value='1'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="pumpOff">
        <input type="hidden" name="id" value="1"/>  
        <input type="hidden" name="status" value='0'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="lightsOn" >
        <input type="hidden" name="id" value="2"/>  
        <input type="hidden" name="status" value='1'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="lightsOff">
        <input type="hidden" name="id" value="2"/>  
        <input type="hidden" name="status" value='0'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="fanOn" >
        <input type="hidden" name="id" value="4"/>  
        <input type="hidden" name="status" value='1'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="fanOff">
        <input type="hidden" name="id" value="4"/>  
        <input type="hidden" name="status" value='0'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="brightsOn" >
        <input type="hidden" name="id" value="3"/>  
        <input type="hidden" name="status" value='1'/>
    </form>
    <form action="Update_Relay_DB.php" method="GET" id="brightsOff">
        <input type="hidden" name="id" value="3"/>  
        <input type="hidden" name="status" value='0'/>
    </form>
    <button class="onButton" name= "subject" type="submit" form="pumpOn" value="pumpOn" >Pump On</button>
    <button class="offButton" name= "subject" type="submit" form="pumpOff" value="pumpOff">Pump Off</button>
    <br>
    <button class="onButton" name= "subject" type="submit" form="lightsOn" value="lightsOn" >Lights On</button>
    <button class="offButton" name= "subject" type="submit" form="lightsOff" value="lightsOff">Lights Off</button>
    <br>
    <button class="onButton" name= "subject" type="submit" form="fanOn" value="fanOn" >Fan On</button>
    <button class="offButton" name= "subject" type="submit" form="fanOff" value="fanOff">Fan Off</button>
    <br>
    <button class="onButton" name= "subject" type="submit" form="brightsOn" value="brightsOn" >Brights On</button>
    <button class="offButton" name= "subject" type="submit" form="brightsOff" value="brightsOff">Brights Off</button>
  </body>
  <br></br>
  <a href="https://charlies-farm-database.herokuapp.com/" >Sensor Data</a>
</html>
