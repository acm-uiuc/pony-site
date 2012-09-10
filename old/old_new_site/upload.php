<?php

include('db.php');

ini_set('post_max_size', '8M');
ini_set('upload_max_filesize', '8M');

if (($_POST['project_owner']) && ($_POST['project_name']) &&
    ($_POST['project_description'])) {

    $owner = $_POST['project_owner'];
    $name = $_POST['project_name'];
    $description = $_POST['project_description'];

    if ( !get_magic_quotes_gpc() ) {
	$owner = addslashes($owner);
	$name = addslashes($name);
	$description = addslashes($description);
    }

    // database connection
    $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
    @mysql_select_db ("sigmil", $conn) OR DIE (mysql_error()); 

    // Store project info
    $sql = "INSERT INTO projects (name, description, owner, active) ";
    $sql.= "VALUES (";
    $sql.= "'{$name}', '{$description}', '{$owner}', TRUE)";
    mysql_query($sql); 

    $project_id = mysql_insert_id();

    foreach ($_FILES as &$file) {
	if ($file['size'] > 0) {
	    $file_tmp_name = $file['tmp_name'];

	    $filename = $file['name'];
	    if ( !get_magic_quotes_gpc() ) {
		$filename = addslashes($filename);
	    }

	    // Open the tmp file and read the data
	    $handle = fopen($file_tmp_name, "r");

	    $contents = addslashes( fread($handle, filesize($file_tmp_name)));
	    fclose($handle);	


	    // Determine content type
	    $content_type = $file['type'];

	    // Store contents into database 
	    $sql = "INSERT INTO project_files (project_id, filename, data, content_type) ";
	    $sql.= "VALUES (";
	    $sql.= "'{$project_id}', '{$filename}', '{$contents}', '{$content_type}')"; 
	    mysql_query($sql); 
	}
    }
   
   header("Location: ../projects.php");
}
else if (($_POST['presenter']) && ($_POST['date']) &&
    ($_POST['talk_description']) && ($_POST['title']) &&
    ($_POST['location'])) {

    $presenter= $_POST['presenter'];
    $date = $_POST['date'];
    $description = $_POST['talk_description'];
    $title = $_POST['title'];
    $location = $_POST['location'];

    if ( !get_magic_quotes_gpc() ) 
    {
	$presenter= addslashes($presenter);
	$date = addslashes($date);
	$description = addslashes($description);
	$title = addslashes($title);
    }

    // database connection
    $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
    @mysql_select_db ("sigmil", $conn) OR DIE (mysql_error()); 
    // Store project info
    $sql = "INSERT INTO talks (name, description, author, location, date_presented) ";
    $sql.= "VALUES (";
    $sql.= "'{$title}', '{$description}', '{$presenter}', '{$location}', STR_TO_DATE('{$date}', '%m/%d/%Y'))";

    mysql_query($sql); 

    $talk_id = mysql_insert_id();

    foreach ($_FILES as &$file) {
	if ($file['size'] > 0) {
	    $file_tmp_name = $file['tmp_name'];

	    $filename = $file['name'];
	    if ( !get_magic_quotes_gpc() ) {
		$filename = addslashes($filename);
	    }

	    // Open the tmp file and read the data
	    $handle = fopen($file_tmp_name, "r");

	    $contents = addslashes( fread($handle, filesize($file_tmp_name)));
	    fclose($handle);	

	    // Determine content type
	    $content_type = $file['type'];

	    // Store contents into database 
	    $sql = "INSERT INTO talk_files (talk_id, filename, data, content_type) ";
	    $sql.= "VALUES (";
	    $sql.= "'{$talk_id}', '{$filename}', '{$contents}', '{$content_type}')"; 
	    mysql_query($sql); 
	}
    }

  header("Location: ../talks.php");
}
else {
    header( "Location: $HTTP_REFERER" );
}
?>
