<!doctype html>

<!-- 
 * Copyright (C) 2016 Francesco Bartolini
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 * 
-->

<html lang="en">
<html>

	<head>
		<meta charset="utf-8" />
		<title>Simulator</title>
		<link rel="stylesheet" href="css/styles.css" type="text/css" media="screen" />
		<link rel="stylesheet" href="css/jquery.datetimepicker.css" type="text/css" media="screen" />
		<script src="js/jquery.min.js"></script>
		<script src="js/jquery.js"></script>
		<script src="js/highcharts.js"></script>
		<script src="js/highcharts-more.js"></script>
		<script src="js/themes/sand-signika.js"></script>
		<script src="js/utils.js"></script>
		<script src="js/jquery.datetimepicker.full.min.js"></script>
	</head>

	<body>
		<header>

    	<h1 id="title">
    		<a href="index.html">Domestic Simulator</a>
    	</h1>
		<nav>
		    	<ul>
		    		<li><a href="index.php">Home</a></li>
		    		
		    		<li><a href="javascript:void(0);" id="l">Display Log</a></li>
		    	</ul>
    	</nav>

    
		</header>
		<!--end header-->
    		
    	<textarea rows="20" cols="50" id="log" readonly></textarea>

		<div id="main">
			
			<div id="content0">
				<div id="gauges">
					<div class="gaubox">
						<div class="inp">Battery power(Wh): <input type="text" name="batt" id="batt" min="0" max="1500"></div>
						<div id="gau1"></div>
					</div>
					
					<div class="gaubox">
						<div class="inp">PV power(W): 
							<select name = "drop_downM" id= "dropDownM"></select>
							<div id="time"><input name = "resInput" id= "resInput">
							<input name = "drop_downH" id= "dropDownH" value="Time"></div>
						</div>
						<div id="gau2"></div>
					</div>
					
					<div class="gaubox">
						<div class="inp">Set Power (W): <input type="text" name="unc" id="unc" min="0" max="3300"></div>
						<div id="gau3"></div>
					</div>
					
					<div class="gauboxSpecial">
						<div class="inp">Max DSO (W):<input type="text" name="dso" id="dso" min="0" max="3300"></div>
						<div id="gau4"></div>
					</div>
				</div>
			</div>
			
			<div id="content"></div>
			
			<div id="content1"></div>
			
		</div>
		
		<footer>
			<p id="note"><a href="http://www.archlinux.org" target="_blank">Design inspired by Archlinux™</a></p>
		</footer>
		
	</body>
	

</html>
