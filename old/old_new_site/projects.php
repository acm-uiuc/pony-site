<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>

<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<meta name="description" content="description"/>
<meta name="keywords" content="keywords"/> 
<meta name="author" content="author"/> 
<link rel="stylesheet" type="text/css" href="default.css" media="screen"/>
<title>SIGMil Projects</title>
</head>

<!-- default margin = default layout -->
<body style="margin: 0 12%;">

<div class="container">
	<?php include 'nav.php' ?>	

	<div class="main">
		<div class="content">
			<?php 
         include('db.php');

         $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
         mysql_select_db("sigmil", $conn) OR DIE (mysql_error());

         $sql = "SELECT * FROM projects WHERE active=TRUE ORDER BY date(created),time(created) DESC";
         $ret = mysql_query($sql, $conn);
         $num = mysql_num_rows($ret);
   
         if ($num > 0) {
            while ($row = mysql_fetch_array($ret)) {
               $id           = $row['id'];
               $date         = $row['created'];
               $owner        = $row['owner'];
               $project_name = $row['name'];
               $description  = $row['description'];
               
               echo "<h1>$project_name</h1>\n";
			      echo "<div class=\"descr\">Project created on: $date by $owner</div>\n";
			      echo "<p>$description</p>\n";

               $sql       = "SELECT * FROM project_files WHERE project_id=$id";
               $ret_files = mysql_query($sql, $conn);
               $num_files = mysql_num_rows($ret);

               if ($num_files > 0) {
                  echo "<ul>";
                  
                  while ($files = mysql_fetch_array($ret_files)) {
                     $file   = $files['filename'];
                     
                     echo "\t<li><a href=\"project_file.php?id=$id&file=$file\">$file</a></li>\n";
                  }
                  
                  echo "</ul>";
               }
               
               echo "<br>\n\n";
			   }
         } else {
              echo "<h1>Float like a butterfly, sting like a bee - hands can't hit what eyes don't see.</h1>\n"; 
         }
         
         mysql_close($conn);
         ?>


		</div>


		<div class="clearer"><span></span></div>
	</div>
		<?php include 'footer.php';?>
</div>
</body>
</html>

