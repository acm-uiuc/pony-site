<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>

<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<meta name="description" content="description"/>
<meta name="keywords" content="keywords"/> 
<meta name="author" content="author"/> 
<link rel="stylesheet" type="text/css" href="../default.css" media="screen"/>
<script type="text/javascript" src="prototype.js"></script>
<script type="text/javascript" src="EditInPlace.js"></script>

<style type="text/css">
/* Define the basic CSS used by EditInPlace */
.eip_mouseover { background-color: #ff9; }
.eip_savebutton { background-color: #36f; color: #fff; }
.eip_cancelbutton { background-color: #000; color: #fff; }
.eip_saving { background-color: #903; color: #fff; padding: 3px; }
.eip_empty { color: #afafaf; }
</style>

<title>SIGMil Talks</title>
</head>

<!-- default margin = default layout -->
<body style="margin: 0 12%;">
<div class="header">
<a href="index.php"><span><img src="img/new_logo.png"></span></a>
</div>

<div class="stripes"><span></span></div>

<div class="container">

<div class="main">
<div class="content">
<?php 
include('db.php');

$conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
mysql_select_db("sigmil", $conn) OR DIE (mysql_error());

$sql = "SELECT * FROM talks ORDER BY id DESC";
$ret = mysql_query($sql, $conn);
$num = mysql_num_rows($ret);

if ($num > 0) {
    while ($row = mysql_fetch_array($ret)) {
	$id           = $row['id'];
	$date         = $row['date_presented'];
	$project_name = $row['name'];
	$author       = $row['author'];
	$location     = $row['location'];
	$description  = $row['description'];

echo <<<END
	<script type="text/javascript">
	Event.observe(window, 'load', init, false);
	function init() {
	    // Over ride some of the default options.
	    EditInPlace.defaults['type'] = 'text';
	    EditInPlace.defaults['save_url'] = 'edit.php';

	    EditInPlace.makeEditable({ id: 'name-$id', ajax_data: { talk_id: $id, } });
	    EditInPlace.makeEditable({ id: 'author-$id', ajax_data: { talk_id: $id, } });
	    EditInPlace.makeEditable({ id: 'location-$id', ajax_data: { talk_id: $id, } });
	    EditInPlace.makeEditable({ id: 'date_presented-$id', ajax_data: { talk_id: $id, } });
	    EditInPlace.makeEditable({ id: 'description-$id', ajax_data: { talk_id: $id, }, type: 'textarea' });
	}
	</script>
END;

	echo "<a name=\"$id\"></a>";
	echo "<h1 id='name-$id'>$project_name</h1>\n";
	echo "<div class=\"descr\">Talk presented on: <span id='date_presented-$id'>$date</span> by <span id='author-$id'>$author</span> at <span id='location-$id'>$location</span></div>\n";
	echo "<p id='description-$id'>$description</p>\n";

	/*
	$sql       = "SELECT * FROM talk_files WHERE talk_id=$id";
	$ret_files = mysql_query($sql, $conn);
	$num_files = mysql_num_rows($ret_files);

	if ($num_files > 0) {
	    echo "<ul>";

	    while ($files = mysql_fetch_array($ret_files)) {
		$file   = $files['filename'];

		echo "\t<li><a href=\"talk_file.php?id=$id&file=$file\">$file</a></li>\n";
	    }

	    echo "</ul>";
	}
	*/

	echo "<br>\n\n";
    }
} else {
    echo "<h1>Float like a butterfly, sting like a bee -- hands can't hit what eyes don't see.</h1>\n"; 
}

mysql_close($conn);

?>


</div>


<div class="clearer"><span></span></div>
</div>
<?php include '../footer.php';?>
</div>
</body>
</html>

