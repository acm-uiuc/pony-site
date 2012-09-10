<?php

if ($_POST['proj_id']) {
    $project_id = $_POST["proj_id"];
    $content = $_POST["content"];
    $field = $_POST["id"];

    if ( !get_magic_quotes_gpc() ) {
	$content = addslashes($content);
    }

    // Feild names are location-id
    $section = explode("-", $field);

    $location = $section[0];

    include('db.php');
    $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
    mysql_select_db("sigmil", $conn) OR DIE (mysql_error());

    // Update sql
    $sql = "UPDATE projects SET $location = '$content' WHERE id = $project_id";
    $ret = mysql_query($sql, $conn);

    print($content);
}
else if ($_POST['talk_id']) {
    $talk_id = $_POST["talk_id"];
    $content = $_POST["content"];
    $field = $_POST["id"];
    
    if ( !get_magic_quotes_gpc() ) {
	$content = addslashes($content);
    }
    
    // Feild names are location-id
    $section = explode("-", $field);
    
    $location = $section[0];

    include('db.php');
    $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
    mysql_select_db("sigmil", $conn) OR DIE (mysql_error());

    // Update sql
    $sql = "UPDATE talks SET $location = '$content' WHERE id = $talk_id";
    $ret = mysql_query($sql, $conn);

    print($content);

}
?>
