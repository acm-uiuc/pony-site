<?php
include ('db.php');

// database connection
$conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
@mysql_select_db ("sigmil", $conn) OR DIE (mysql_error());

$id = mysql_real_escape_string($_GET["id"]);
$file = mysql_real_escape_string($_GET["file"]);

$sql = "SELECT * FROM project_files WHERE project_id =". $id .
       "&& filename = '" . $file . "'";

$result = mysql_query($sql, $conn);

$row = @mysql_fetch_array($result);
$data = $row["data"];
$content_type = $row["content_type"];


if ($content_type) {
    header("Content-type: $content_type");
}
else
{
    header("Content-type: text/html");
}

print $data;
exit();
?>
