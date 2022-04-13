<?php
  include 'database.php';
  
  if (!empty($_POST)) {
    
        
    $sql = "SELECT * FROM relays ";
    $result = mysqli_query($conn,$sql);
    $resultCheck = mysqli_num_rows($result);

    if($resultCheck >0){
        while($row = mysqli_fetch_assoc($result)){
           echo "id:".$row['id']; 
           echo "status:".$row['status']; 
        }
    }

    
  }
?>