<?php
class dht11{
 public $link='';
 function __construct($name,$mobile,$lat,$lon){
  $this->connect();
  $this->storeInDB($name,$mobile,$lat,$lon);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'iottest1') or die('Cannot select the DB');
 }
 
 function storeInDB($name,$mobile,$lat, $lon){
  $query = "insert into dht11 set name='".$name."', mobile='".$mobile."',lon='".$lon."', lat='".$lat."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }

}

if($_GET['name'] != '' and $_GET['mobile'] != '' and $_GET['lat'] != '' and  $_GET['lon'] != ''){
 $dht11=new dht11($_GET['name'],$_GET['mobile'],$_GET['lat'],$_GET['lon']);
}
?>