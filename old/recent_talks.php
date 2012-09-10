<p>Recent Talks:</p>
<ul>
<?php
include('db.php');

$conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
mysql_select_db("sigmil", $conn) OR DIE (mysql_error());

$sql = "SELECT * FROM talks ORDER BY date(date_presented) DESC LIMIT 0,3";
$ret = mysql_query($sql, $conn);
$num = mysql_num_rows($ret);

if ($num > 0) {
   while ($row = mysql_fetch_array($ret)) {
      $id   = $row['id'];
      $name = $row['name'];

      echo "<li><a href=\"talks.php?year=all#$id\">$name</a></li>\n";
   }
} else {
   echo "<center>No Recent Talks</center>";
}

mysql_close($conn);
?>

</ul>

