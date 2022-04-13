

<?php

include 'database.php';


if(!$conn){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success!<br><br>";

if($_SERVER['REQUEST_METHOD']==='POST'){
    $temp_air = test_input($_POST["temp_air"]);
    $humidity = test_input($_POST["humidity"]);
    $temp_water = test_input($_POST["temp_water"]);
    $pH = test_input($_POST["pH"]);
    $ec= test_input($_POST["ec"]);
    $ph_up_pump= test_input($_POST["ph_up_pump"]);
    $ph_down_pump= test_input($_POST["ph_down_pump"]);
    $pmp_a= test_input($_POST["pmp_a"]);
    $pmp_b= test_input($_POST["pmp_b"]);


    $query = "INSERT INTO sensor_data (temp_air, humidity,temp_water,pH,ec,ph_up_pump,ph_down_pump,pmp_a,pmp_b) VALUES ('".$temp_air."', '".$humidity."','".$temp_water."','".$pH."','".$ec."','".$ph_up_pump."','".$ph_down_pump."','".$pmp_a."','".$pmp_b."')";
    $result = mysqli_query($conn,$query);

    echo "Insertion Success!<br>"; 
}
if($_SERVER['REQUEST_METHOD']==='GET'){
    
    $SQL = "SELECT * FROM sensor_data;";
    echo "GET request!<br>"; 
 }

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}