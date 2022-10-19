#!/usr/bin/php-cgi
<?php

	if (isset($_POST)){
		echo "<h2>POST :</h2>";
		echo "<p>";
		var_dump($_POST);
		echo "</p>";
	}
	if (isset($_FILES)){
		echo "<h2>FILES :</h2>";
		echo "<p>";
		var_dump($_FILES);
		echo "</p>";
	}
?>