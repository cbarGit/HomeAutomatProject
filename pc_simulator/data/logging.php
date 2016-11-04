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

class Logging {
    // declare log file and file pointer as private properties
    private $log_file, $fp;
    // set log file (path and name)
    public function lfile($path) {
        $this->log_file = $path;
    }
    // write message to the log file
    public function lwrite($message) {
        // if file pointer doesn't exist, then open log file
        if (!is_resource($this->fp)) {
            $this->lopen();
        }
        // define script name
        $script_name = pathinfo($_SERVER['PHP_SELF'], PATHINFO_FILENAME);
        // define current time and suppress E_WARNING if using the system TZ settings
        // (don't forget to set the INI setting date.timezone)
        $time = @date('[d/M/Y - H:i:s]');
        // write current time, script name and message to the log file
        fwrite($this->fp, "$time ($script_name) $message" . PHP_EOL);
    }
    // close log file (it's always a good idea to close a file when you're done with it)
    public function lclose() {
        fclose($this->fp);
    }
    // open log file (private method)
    private function lopen() {
        $log_file_default = 'log.txt';
        // define log file from lfile method or use previously set default
        $lfile = $this->log_file ? $this->log_file : $log_file_default;
        // open log file for writing only and place file pointer at the end of the file
        // (if the file does not exist, try to create it)
        $this->fp = fopen($lfile, 'a') or exit("Can't open $lfile!");
    }
}
// Logging class initialization
$log = new Logging();
 
// set path and name of log file (optional)
$log->lfile('log.txt');

$val = (int)$_REQUEST["val"];

// write message to the log file
if($val == 0){
	$log->lwrite('Load energy TURNED ON. PV energy selected.');	
}elseif($val == 1){
	$log->lwrite('Load energy TURNED ON. Battery discharging!');	
}elseif($val == 2){
	$log->lwrite('Load energy TURNED ON. PV+DSO.');	
}elseif($val == 3){
	$log->lwrite('Load energy TURNED OFF. Requested energy higher then limits.');	
}elseif($val == 4){
	$log->lwrite('Load energy TURNED ON. Help from Battery.');	
}elseif($val == 5){
	$log->lwrite('Load energy TURNED OFF. Requested power greater then DSO.');	
}elseif($val == 6){
	$log->lwrite('Load energy TURNED ON. Enough power from DSO.');	
}elseif($val == 7){
	$log->lwrite('Load energy TURNED OFF. Definitely Over Bounds.');	
}else{
	$log->lwrite('Turn off load, simulator not reachable.');	
}

// close log file
$log->lclose();
?>
