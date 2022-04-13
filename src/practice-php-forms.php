<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="refresh" content="5" >
    <link rel="stylesheet" type="text/css" href="style.css" media="screen"/>

	<title> Sensor Data </title>

</head>

<body>


<form method="GET">
        <input type="hidden" name="id" value=1>
        <button>Submit</button>
    </form>
<?php
    $id = $_GET['id'];
    echo $id."relay number";
    //echo $_POST['id'];
?>

</body>
</html>