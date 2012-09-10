<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>

<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<meta name="description" content="SIGPony is a University of Illinois
Urbana-Champaign (UIUC) student security research and projects group."/>
<meta name="keywords" content="SIGPony, Security, Computers, Hardware,
Projects, UIUC, University of Illinois, RevEng, Reverse Engineering, Trust,
Student"/> 
<meta name="author" content="SIGPony"/> 
<link rel="stylesheet" type="text/css" href="default.css" media="screen"/>
<title>SIGPony</title>
</head>

<!-- default margin = default layout -->
<body style="margin: 0 12%;">

<div class="container">
	<?php include 'nav.php' ?>	

	<div class="main">
	
		<div class="left">

			<div class="content">
				<h1>Welcome</h1>
				<div class="descr"></div>

				<p>Welcome to SIGPony</p>

				<p>The Special Interest Group for Electronic Security and Intrusion (SIGPony) at
				the University of Illinois Urbana - Champaign (UIUC) does security
				"stuff". Formally we're focused on network and computer security
				research and education. We're always looking for new members,
				including freshman.</p>

				<p>If you have any interest or knowledge with the following random
				sampling of the sorts of words and acronyms we deal with you
				should hang out with us:</p>

				<ul>
					<li>TCP/IP, ASM, POTS, ARP, GSM, SIM Card,</li>
					<li>DOCSIS, CO, ANI, Blackhat, Whitehat, CDMA,</li>
					<li>TDMA, FM, FCC, PGP, AES, RMS, RFID, SYN,</li>
					<li>ACK, DOS, ISO 7816, Stack, Heap, Buffer,</li>
					<li>802.11, ATM, PIC, RPC, PVC, TTL, EEPROM </li>
				</ul>

				<p>Until then feel free to contact sigpony@acm.uiuc.edu with any
				questions or comments.</p>
				

				<h1>For Fun</h1>
				<div class="descr">What does it do?</div>

				<p></p>	
				<code>
File: mystery.pl

#!/bin/perl -sp0777i&lt;X+d*lMLa^*lN%0]dsXx++lMlN/dsM0&lt;j]dsj
$/=unpack('H*',$_);$_=`echo 16dio\U$k&quot;SK$/SM$n\EsN0p[lN*1
lK[d2%Sa2/d0$^Ixp&quot;|dc`;s/\W//g;$_=pack('H*',/((..)*)$/)

Usage
--- 
% chmod 700 mystery.pl
% echo "Try this!" | mystery.pl -k=10001 -n=1967cb529 &gt; mystery.output
% mystery.pl -d -k=ac363601 -n=1967cb529 &lt; mystery.output
				</code>
			</div>

		</div>

		<div class="right">

			<div class="subnav">

				<h1>Meetings</h1>
				<p>SIGPony meets on Fridays at 7PM in 2405 Siebel Center.</p>

				<?php include 'recent_talks.php';	?>
				
<? /*<h1>SILC Server</h1>
				<p>Want to get information, ask a question, or hang out with
				SIGPony members online?  Check out our SILC server</p>

				<p>Server: sig.acm.uiuc.edu:80<br>
				Room: sigmil</p>

				<p>Possible Clients:</p>
				<ul>
					<li><a href="http://en.wikipedia.org/wiki/SILC_%28protocol%29">What is SILC</a></li>
					<li><a href="http://silky.sourceforge.net/">Silky (Windows, Linux)</a></li>
					<li><a href="http://pidgin.im/">Pidgin (Windows, Linux, *BSD)</a></li>
					<li><a href="http://colloquy.info/">Colloquy (Mac OS X)</a></li>
				</ul>
 */ ?>
				
				<h1>Events & News</h1>
				<ul>
					<li><a href="http://www.acm.uiuc.edu/events.shtml">ACM Open House</a></li>
					<li><a href="http://www.cs.ucsb.edu/~vigna/CTF">CTF</a></li>
					<li><a href="http://eoh.ec.uiuc.edu">Engineering Open House</a></li>
				</ul>

				<p>You can also sign up for our <a
				href='https://www-s.acm.uiuc.edu/cgi-bin/mailman/listinfo/sigmil-l'>mailing
				list</a> to receive meeting annoucements and
				other news.</p>

			</div>

		</div>

		<div class="clearer"><span></span></div>

	</div>
		<?php include 'footer.php';?>

</div>
</body>
</html>
