<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>

<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<meta name="description" content="description"/>
<meta name="keywords" content="keywords"/> 
<meta name="author" content="author"/> 
<link rel="stylesheet" type="text/css" href="../default.css" media="screen"/>
<title>SIGMil Projects</title>
<script src="../scripts/multifile_compressed.js"></script>
<link href="../css/datepicker.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="../js/datepicker.js"></script>
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
			<h1>Upload a New Talk</h1>
			<div class="descr"></div>

			<form enctype="multipart/form-data" action="upload.php" method = "post">
			    <table border=0 cellspacing=5>
			    <tr><td>Presenter:</td><td><input type="text" name="presenter"></td></tr>
			    <tr><td><label for="dp-normal-1">Date :</label></td><td>
			    <input type="text" class="format-m-d-y divider-slash highlight-days-67 transparency" name="date" value="" maxlength="10"/></td></tr>
			    <tr><td>Location:</td><td><input type="text" name="location" value="2405 Siebel Center"></td></tr>
			    <tr><td>Title:</td><td><input type="text" name="title" size=81></td></tr>
			    <tr><td>Description:</td><td><textarea name="talk_description" rows=20 cols=80></textarea><td></tr>

			    <tr><td>Attach File:</td><td><input id="my_file_element" type="file" name="file_1" ></td></tr>
			    <tr><td></td><td><div id="files_list" class="files"></div></td>
			<script>
			var multi_selector = new MultiSelector(
			document.getElementById( 'files_list' ), 3 );
			multi_selector.addElement( document.getElementById( 'my_file_element' ) );
			</script>

			    <tr><td></td><td><input type="submit" value="Upload Talk"></td></tr>
			    </table>
			</form>
		</div>


		<div class="clearer"><span></span></div>
	</div>
		<?php include '../footer.php';?>
</div>
</body>
</html>

