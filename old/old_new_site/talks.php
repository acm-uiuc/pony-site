<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>

<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<meta name="description" content="description"/>
<meta name="keywords" content="keywords"/> 
<meta name="author" content="author"/> 
<link rel="stylesheet" type="text/css" href="default.css" media="screen"/>
<title>SIGMil Talks</title>
</head>

<!-- default margin = default layout -->
<body style="margin: 0 12%;">

<div class="container">
	<?php include 'nav.php' ?>	

	<div class="main">
      <div class="left">
		   <div class="content">
			<?php 
         include('db.php');

         $year = 0;
         $show_all = 0;
         
         if (isset($_GET['year'])) {
            if (strcmp($_GET['year'], "all") == 0) {
               $show_all = 1;
            }
            
            $year = ereg_replace("[^0-9]", "", $_GET['year']);
         }
         
         if ($year > date("Y") || $year < 2002) {
            $year = date("Y");
         }
         
         $conn = mysql_connect($db_svr, $db_user, $db_pwd) OR DIE (mysql_error());
         mysql_select_db("sigmil", $conn) OR DIE (mysql_error());
         
         if ($show_all == 1) {
            $sql = "SELECT * FROM talks ORDER BY date(date_presented) DESC";
         } else {
            $sql = "SELECT * FROM talks WHERE year(date_presented)=$year ORDER BY date(date_presented) DESC";
         }
         
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
               
               echo "<a name=\"$id\"></a>";
               echo "<h1>$project_name</h1>\n";
			      echo "<div class=\"descr\">Talk presented on: $date by $author at $location</div>\n";
			      echo "<p>$description</p>\n";

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
               
               echo "<br>\n\n";
			   }
         } else {
              echo "<h1>Float like a butterfly, sting like a bee - hands can't hit what eyes don't see.</h1>\n"; 
         }
         
         mysql_close($conn);
         ?>

		   </div>
      </div>

      <div class="right">
         <div class="subnav">
            <h1>Previous Year's Talks</h1>
            
            <ul>
            <?php
            $current_year = date("Y");

            for ($i = $current_year; $i > 2001; $i--) {
               echo "\t<li><a href=\"talks.php?year=$i\">$i</a></li>\n";
            }
            ?>
               <li><a href="talks.php?year=all">show all</a></li>
            </ul>
         </div>

      </div>

		<div class="clearer"><span></span></div>
	</div>
		<?php include 'footer.php';?>
</div>
</body>
</html>

