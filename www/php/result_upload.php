<!DOCTYPE html>
<html>
	<head>
		<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Rubik">
		<title>WebServ</title>
		<style>
			body {
			display: flex;
			
			min-height: 100vh;
			background: radial-gradient(ellipse at bottom, #0d1d31 0%, #0c0d13 100%);
			overflow: hidden;
			font-family: 'Rubik';
			}
			h1, h2{
				margin-top: 10px;
				margin-left: 10px;
				font-weight: bold;
			}
			h1{
				color: white;
				font-size: 65px;
				/* margin-top: -80px;
				margin-left: 10px; */
			}
			p{
				color: #607d8b;
				margin-top: 200px;
				margin-left: 10px;
				font-size: 35px;
			}
			.head{
				margin-top: -10px;
			}
			a{
				color: white;
				font-size: 30px;
				display:block;
				height:100%;
				width:100%;
				border: 5px solid #607d8b;
				border-radius: 10px;
				/* background-color: white; */
				/* filter: blur(8px); */
				/* -webkit-filter: blur(8px); */
			}
			.stack{
				margin-top: 100px;
			}
			</style>
	</head>
	<body>
		<h1>Files received</h1>
		<h2>The content interpreted by php is:</h2>
		<center>
		<p>
			<?php
				if (isset($_FILES)){
					var_dump($_FILES);
				}
			?>
		</p>
		</center>
	</body>
</html>