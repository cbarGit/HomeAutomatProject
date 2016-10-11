<?php

/* 
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
 */

//create array
$contentsDecoded = array();

//get value from appropriate file
if ($_GET["val"] == 'unc'){
	$jsonFile = file_get_contents('unc.json');
} elseif($_GET["val"] == 'pv'){
	$jsonFile = file_get_contents('pvFinal.json');
} elseif($_GET["val"] == 'dso'){
	$jsonFile = file_get_contents('dso.json');
} else {
	$jsonFile = file_get_contents('batt.json');
}

//Encode the array back into a JSON string.
$contentsDecoded = json_decode($jsonFile);
//encode json
$json = json_encode($contentsDecoded);

header('Content-Type: application/json');
//send json
echo $json; 

?>

