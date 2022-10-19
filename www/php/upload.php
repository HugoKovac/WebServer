<!DOCTYPE html>
<html>
	<head>
		<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Rubik">
		<title>WebServ</title>
		<style>
			body {
			display: flex;
			/* justify-content: center; */
			/* align-items: center; */
			min-height: 100vh;
			background: radial-gradient(ellipse at bottom, #0d1d31 0%, #0c0d13 100%);
			overflow: hidden;
			font-family: 'Rubik';
			}
			h1, h2{
				margin-top: 20px;
				margin-left: 20px;
				font-weight: bold;
			}
			h1{
				color: white;
				font-size: 65px;
			}

			</style>
	</head>
	<body>
		<h1>Upload a File</h1>

		<form enctype="multipart/form-data" action="result_upload.php" method="POST">
			<!-- MAX_FILE_SIZE must precede the file input field -->
			<input type="hidden" name="MAX_FILE_SIZE" value="3000" />
			<!-- Name of input element determines name in $_FILES array -->
			Send this file: <input name="userfile" type="file" />
			<input type="submit" value="Send File" />
		</form>

	</body>
</html>