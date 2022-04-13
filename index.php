<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="refresh" content="300" >
    <link rel="stylesheet" type="text/css" href="src/style.css" media="screen"/>
	<title> Sensor Data </title>
</head>
<body>

    <h1>SENSOR DATA</h1>
    <a href="https://charlies-farm-database.herokuapp.com/src/relay_toggle.php">Relay Controls</a>
<?php
include_once 'src/database.php';    
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$sql = "SELECT id, temp_air, humidity, temp_water, pH, ec, ph_up_pump, ph_down_pump, pmp_a, pmp_b, date FROM sensor_data ORDER BY id DESC LIMIT 100"; /*select items to display from the sensordata table in the data base*/
$result = mysqli_query($conn,$sql);
$resultCheck = mysqli_num_rows($result);
echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <th>ID</th> 
        <th>Date - Time</th> 
        <th>Temperature &deg;C</th> 
        <th>Humidity &#37;</th>
        <th>Water Temp &deg;C</th>
        <th>pH</>
        <th>ec</th>
        <th>pH UP Pump</th>
        <th>ph DOWN Pump</th>
        <th>Nutrient A Pump</th>
        <th>Nutrient B Pump</th>     
      </tr>';
if ($resultCheck >0) {
    while ($row = mysqli_fetch_assoc($result)) {
        $row_id = $row["id"];
        $row_date = $row["date"];
        $row_temp = $row["temp_air"];
        $row_humidity = $row["humidity"];
        $row_water_temp = $row["temp_water"];
        $row_pH = $row["pH"];
        $row_ec = $row["ec"];
        $row_ph_up_pump = $row["ph_up_pump"];
        $row_ph_down_pump = $row["ph_down_pump"];
        $row_pmp_a = $row["pmp_a"];
        $row_pmp_b = $row["pmp_b"];
        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_date . '</td> 
                <td>' . $row_temp . '</td> 
                <td>' . $row_humidity . '</td> 
                <td>' . $row_water_temp . '</td> 
                <td>' . $row_pH . '</td>
                <td>' . $row_ec . '</td>
                <td>' . $row_ph_up_pump . '</td>
                <td>' . $row_ph_down_pump . '</td>
                <td>' . $row_pmp_a . '</td>
                <td>' . $row_pmp_b . '</td>
              </tr>';   
    }
    $result->free();
}
$conn->close();
?> 
</body>

    
</html>
