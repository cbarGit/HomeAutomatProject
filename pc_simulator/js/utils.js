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

$(document).ready(function(){
	$('#batt').val(200);
	$('#dso').val(3300);
	$('#unc').val(300);
	g1Batt();
	g2PV();
	g3Unc();
	g4Lamp();
	Loads();
	RO();
    textdata();
    /*setInterval(
        function() { 
			textdata(); 
			},
    30000);*/

    $("#l").click(function() {
		textdata();
        $("#log").slideToggle(100);
        $('#log').scrollTop($('#log')[0].scrollHeight);
    });

	var value1;
    var json = $.getJSON("data/pv.json",function(obj)
   {
	   $.each(obj,function(key,value)
	   {
		   var option = $('<option />').val(key).text(key);
		   $("#dropDownM").append(option);
			//console.log(value);
		});
		
		//$('#pv').val($('#dropDownH').val(value1));		
    });
	
	$('#dropDownH').datetimepicker({
		datepicker:false,
		format:'H:i',
		allowTimes: ["4:52","5:07","5:22","5:37","5:52","6:07","6:22","6:37","6:52","7:07","7:22","7:37","7:52","8:07","8:22","8:37","8:52","9:07","9:22","9:37","9:52","10:07","10:22","10:37","10:52","11:07","11:22","11:37","11:52","12:07","12:22","12:37","12:52","13:07","13:22","13:37","13:52","14:07","14:22","14:37","14:52","15:07","15:22","15:37","15:52","16:07","16:22","16:37","16:52","17:07","17:22","17:37","17:52","18:07","18:22","18:37","18:52","19:07","19:22"]
	});
	
	
	
	function getVal(){
		var time = $('#dropDownH').val();
		var month = $('#dropDownM').val();
		var res;
		//console.log(json);
		$.each(json, function(key, value) {
			if (value[month]){
				//console.log(value[month]);
				var o = value[month];
				$.each(o, function(key1, value1) {
					if (key1 == time){
						//console.log(value1);
					res = value1;
					}
				});
			}
		});
		return res;
	}
	
	$('#dropDownH').on('change', function() { 
		$('#resInput').val(getVal());
	});
	
	
});//end of $(document).ready


function showValue(num){
           var result = document.getElementById('limit');     
           result.innerHTML = num;
}

function textdata(){
    $.ajax({
        url : "data/log.txt",
        dataType: "text",
        success : function (data) {
            $("#log").text(data);
        }
    });
}


//Battery gauge
function g1Batt() {

    var options = {

        chart: {
            type: 'gauge',
            renderTo: 'gau1',
            plotBackgroundColor: null,
            plotBackgroundImage: null,
            plotBorderWidth: 0,
            plotShadow: false
        },

        title: {
            text: 'Battery Meter'
        },

        pane: {
            startAngle: -150,
            endAngle: 150,
            background: [{
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#FFF'],
                        [1, '#333']
                    ]
                },
                borderWidth: 0,
                outerRadius: '109%'
            }, {
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#333'],
                        [1, '#FFF']
                    ]
                },
                borderWidth: 1,
                outerRadius: '107%'
            }, {
                // default background
            }, {
                backgroundColor: '#DDD',
                borderWidth: 0,
                outerRadius: '105%',
                innerRadius: '103%'
            }]
        },

        // the value axis
        yAxis: {
            min: 0,
            max: 1500,

            minorTickInterval: 'auto',
            minorTickWidth: 1,
            minorTickLength: 10,
            minorTickPosition: 'inside',
            minorTickColor: '#666',

            tickPixelInterval: 30,
            tickWidth: 2,
            tickPosition: 'inside',
            tickLength: 10,
            tickColor: '#666',
            labels: {
                step: 2,
                rotation: 'auto'
            },
            title: {
                text: 'Wh'
            },
            plotBands: [{
                from: 0,
                to: 500,
                color: '#55BF3B' // green
            }, {
                from: 500,
                to: 1000,
                color: '#DDDF0D' // yellow
            }, {
                from: 1000,
                to: 1500,
                color: '#DF5353' // red
            }]
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Power',
            tooltip: {
                valueSuffix: ' Wh'
            }
        }]
    };
    var chart;
    var batt_value = $('#batt').val();
    $.getJSON('data/batt.json.php?batt='+batt_value, function(data) {
        options.series[0].data = [data["power"]];
        chart = new Highcharts.Chart(options);
    });
    setInterval(function () {
        $.getJSON('data/battRead.json.php', function(data) {
            var point = chart.series[0].points[0];
            point.update([data["power"]]);
			$('#batt').val([data["power"]]);
        })
    }, 30000);
}

//PV gauge
function g2PV() {

    var options = {

        chart: {
            type: 'gauge',
            renderTo: 'gau2',
            plotBackgroundColor: null,
            plotBackgroundImage: null,
            plotBorderWidth: 0,
            plotShadow: false
        },

        title: {
            text: 'PV Meter'
        },

        pane: {
            startAngle: -150,
            endAngle: 150,
            background: [{
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#FFF'],
                        [1, '#333']
                    ]
                },
                borderWidth: 0,
                outerRadius: '109%'
            }, {
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#333'],
                        [1, '#FFF']
                    ]
                },
                borderWidth: 1,
                outerRadius: '107%'
            }, {
                // default background
            }, {
                backgroundColor: '#DDD',
                borderWidth: 0,
                outerRadius: '105%',
                innerRadius: '103%'
            }]
        },

        // the value axis
        yAxis: {
            min: 0,
            max: 2100,

            minorTickInterval: 'auto',
            minorTickWidth: 1,
            minorTickLength: 10,
            minorTickPosition: 'inside',
            minorTickColor: '#666',

            tickPixelInterval: 30,
            tickWidth: 2,
            tickPosition: 'inside',
            tickLength: 10,
            tickColor: '#666',
            labels: {
                step: 2,
                rotation: 'auto'
            },
            title: {
                text: 'W'
            },
            plotBands: [{
                from: 0,
                to: 700,
                color: '#55BF3B' // green
            }, {
                from: 700,
                to: 1400,
                color: '#DDDF0D' // yellow
            }, {
                from: 1400,
                to: 2100,
                color: '#DF5353' // red
            }]
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Power',
            tooltip: {
                valueSuffix: ' W'
            }
        }]

    };
    var chart;
    var pv_value = parseInt($('#resInput').val());
    //$("#pv").val($("#pv_value").val());
    $.getJSON('data/pv.json.php?pv='+pv_value.toString(), function(data) {
        options.series[0].data = [data["power"]];
        chart = new Highcharts.Chart(options);
    });
    setInterval(function () {
        var pv_value = parseInt($("#resInput").val());
        $.getJSON('data/pv.json.php?pv='+pv_value.toString(), function(data) {
            var point = chart.series[0].points[0];
            point.update([data["power"]]);
        })
    }, 30000);
}

//Uncontrolled Load gauge
function g3Unc() {

    var options = {

        chart: {
            type: 'gauge',
            renderTo: 'gau3',
            plotBackgroundColor: null,
            plotBackgroundImage: null,
            plotBorderWidth: 0,
            plotShadow: false
        },

        title: {
            text: 'Uncontrolled Load'
        },

        pane: {
            startAngle: -150,
            endAngle: 150,
            background: [{
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#FFF'],
                        [1, '#333']
                    ]
                },
                borderWidth: 0,
                outerRadius: '109%'
            }, {
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#333'],
                        [1, '#FFF']
                    ]
                },
                borderWidth: 1,
                outerRadius: '107%'
            }, {
                // default background
            }, {
                backgroundColor: '#DDD',
                borderWidth: 0,
                outerRadius: '105%',
                innerRadius: '103%'
            }]
        },

        // the value axis
        yAxis: {
            min: 0,
            max: 3300,

            minorTickInterval: 'auto',
            minorTickWidth: 1,
            minorTickLength: 10,
            minorTickPosition: 'inside',
            minorTickColor: '#666',

            tickPixelInterval: 30,
            tickWidth: 2,
            tickPosition: 'inside',
            tickLength: 10,
            tickColor: '#666',
            labels: {
                step: 2,
                rotation: 'auto'
            },
            title: {
                text: 'W'
            },
            plotBands: [{
                from: 0,
                to: 1100,
                color: '#55BF3B' // green
            }, {
                from: 1100,
                to: 2200,
                color: '#DDDF0D' // yellow
            }, {
                from: 2200,
                to: 3300,
                color: '#DF5353' // red
            }]
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Power',
            //data: [80],
            tooltip: {
                valueSuffix: ' W'
            }
        }]

    };

    var chart;
    var unc_value = $('#unc').val();
    $.getJSON('data/unc.json.php?unc='+unc_value, function(data) {
        options.series[0].data = [data["power"]];
        chart = new Highcharts.Chart(options);
    });
    setInterval(function () {
        var unc_value = $("#unc").val();
        $.getJSON('data/unc.json.php?unc='+unc_value, function(data) {
            var point = chart.series[0].points[0];
            point.update([data["power"]]);
        })
    }, 30000);
}

//Lamp gauge
function g4Lamp() {

    var options = {

        chart: {
            type: 'gauge',
            renderTo: 'gau4',
            plotBackgroundColor: null,
            plotBackgroundImage: null,
            plotBorderWidth: 0,
            plotShadow: false
        },

        title: {
            text: 'Lamp Meter'
        },

        pane: {
            startAngle: -150,
            endAngle: 150,
            background: [{
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#FFF'],
                        [1, '#333']
                    ]
                },
                borderWidth: 0,
                outerRadius: '109%'
            }, {
                backgroundColor: {
                    linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                    stops: [
                        [0, '#333'],
                        [1, '#FFF']
                    ]
                },
                borderWidth: 1,
                outerRadius: '107%'
            }, {
                // default background
            }, {
                backgroundColor: '#DDD',
                borderWidth: 0,
                outerRadius: '105%',
                innerRadius: '103%'
            }]
        },

        // the value axis
        yAxis: {
            min: 0,
            max: 600,

            minorTickInterval: 'auto',
            minorTickWidth: 1,
            minorTickLength: 10,
            minorTickPosition: 'inside',
            minorTickColor: '#666',

            tickPixelInterval: 30,
            tickWidth: 2,
            tickPosition: 'inside',
            tickLength: 10,
            tickColor: '#666',
            labels: {
                step: 2,
                rotation: 'auto'
            },
            title: {
                text: 'W'
            },
            plotBands: [{
                from: 0,
                to: 200,
                color: '#55BF3B' // green
            }, {
                from: 200,
                to: 400,
                color: '#DDDF0D' // yellow
            }, {
                from: 400,
                to: 600,
                color: '#DF5353' // red
            }]
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Power',
            //data: [80],
            tooltip: {
                valueSuffix: ' W'
            }
        }]

    };

    var chart;
    options.series[0].data = [0];
    chart = new Highcharts.Chart(options);
    $.getJSON('data/lamp.json.php')
		.done(function(data) {
		//options.series[0].data = [data["power"]];
        //chart = new Highcharts.Chart(options);
        var point = chart.series[0].points[0];
        point.update([data["power"]]);
    	})
		.always(function(data) {
		//options.series[0].data = [0];
        //chart = new Highcharts.Chart(options);
        var point = chart.series[0].points[0];
        point.update([0]);
		console.log("Lamp .getJSON() Error: NO DATA FROM 'data/lamp.json.php' ");
    	});
    setInterval(function () {
        $.getJSON('data/lamp.json.php')
			.done(function(data) {
			var point = chart.series[0].points[0];
            point.update([data["power"]]);
        	})
			.fail(function(data) {
			console.log("Lamp setInterval() Error: NO DATA FROM 'data/lamp.json.php' ");
			var point = chart.series[0].points[0];
            point.update([0]);
        });
    }, 30000);
}     


//Loads graphics
function Loads() {
        Highcharts.setOptions({
            global: {
                useUTC: false
            }
        });

        $('#content').highcharts({
            chart: {
                type: 'spline',
                animation: Highcharts.svg, // don't animate in old IE
                marginRight: 10,
                events: {
                    load: function () {
                        // set up the updating of the chart each second
                        var series = this.series[0];
                        var series1 = this.series[1];
                        var series2 = this.series[2];
                        var series3 = this.series[3];
                        setInterval(function () {
                            var unc_value = $("#unc").val();
                            var pv_value = parseInt($("#resInput").val());
                            var batt_value = $("#batt").val();
                            $.getJSON('data/lamp.json.php')
							.done(function(data) {
                                var y = data["power"];
                                var x = (new Date()).getTime(); // current time
                                series.addPoint([x, y], true, true);
                            })
							.fail(function(data) {
                                var y = 0;
                                var x = (new Date()).getTime(); // current time
                                series.addPoint([x, y], true, true);
								console.log("Lamp Spline error");
							});
                            $.getJSON('data/battRead.json.php')
							.done(function(data) {
                                var y = data["power"];
                                var x = (new Date()).getTime(); // current time
                                series1.addPoint([x, y], true, true);
                            })
							.fail(function(data) {
                                var y = 0;
                                var x = (new Date()).getTime(); // current time
                                series1.addPoint([x, y], true, true);
								console.log("Batt Spline error");
							});
                            $.getJSON('data/pv.json.php?pv='+pv_value.toString())
							.done(function(data) {
                                var y = data["power"];
                                var x = (new Date()).getTime(); // current time
                                series2.addPoint([x, y], true, true);
                            })
							.fail(function(data) {
                                var y = 0;
                                var x = (new Date()).getTime(); // current time
                                series2.addPoint([x, y], true, true);
								console.log("PV Spline error");
							});
                            $.getJSON('data/unc.json.php?unc='+unc_value)
							.done(function(data) {
                                var y = data["power"];
                                var x = (new Date()).getTime(); // current time
                                series3.addPoint([x, y], true, true);
                            })
							.fail(function(data) {
                                var y = 0;
                                var x = (new Date()).getTime(); // current time
                                series3.addPoint([x, y], true, true);
								console.log("Unc Spline error");
							});
                        }, 60000);
                    }
                }
            },
            title: {
                text: 'Live Power History'
            },
            xAxis: {
                type: 'datetime',
                tickPixelInterval: 30000,
                tickInterval: 360000
            },
            yAxis: [{
                title: {
                    text: 'W'
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            }],
            plotOptions: {
            spline: {
                lineWidth: 4,
                states:{
                    hover:{
                    lineWidth: 5
                    }
                },
                marker: {
                    enabled: false
                    }
                }
            },
            tooltip: {
                formatter: function () {
                    return '<b>' + this.series.name + '</b><br/>' +
                        Highcharts.dateFormat('Value at %H:%M:%S', this.x) + '<br/>' +
                        Highcharts.numberFormat(this.y, 2);
                }
            },
            legend: {
                enabled: true
            },
            credits: {
                enabled: false
            },
            exporting: {
                enabled: false
            },
            series: [{
                name: 'Lamp Power',
                data: (function () {
                    /*var myval;
                    $.getJSON('data/lamp.json.php', function(data) {
                        myval = data["power"];
                    });*/
                    // generate an array of data
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: 0
                        });
                    }
                    return data;
                }())
            },{
                name: 'Battery Power',
                data: (function () {
                    /*var myval;
                    $.getJSON('data/batt.json.php', function(data) {
                        myval = data["power"];
                    });*/
                    // generate an array of data
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: 0
                        });
                    }
                    return data;
                }())
            },{
                name: 'PV Power',
                data: (function () {
                    /*var myval;
                    $.getJSON('data/lamp.json.php', function(data) {
                        myval = data["power"];
                    });*/
                    // generate an array of data
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: 0
                        });
                    }
                    return data;
                }())
            },{
                name: 'Uncontrolled Power',
                data: (function () {
                    /*var myval;
                    $.getJSON('data/lamp.json.php', function(data) {
                        myval = data["power"];
                    });*/
                    // generate an array of data
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: 0
                        });
                    }
                    return data;
                }())
            }]
        });
}

//Requested/Offered graphic
function RO() {

        Highcharts.setOptions({
            global: {
                useUTC: false
            }
        });

        $('#content1').highcharts({
            chart: {
                type: 'spline',
                animation: Highcharts.svg, // don't animate in old IE
                marginRight: 10,
                events: {
                    load: function () {
                        // set up the updating of the chart each second
                        var series = this.series[0];
                        var series1 = this.series[1];
						var tot_value;
						var tot_value1;
                        setInterval(function () {
							
							var dso = parseInt($("#dso").val());
							var pv = parseInt($("#pv").val());
							var batt = parseInt($("#batt").val());
							var unc = parseInt($("#unc").val());							

							if(isNaN(dso)){
								var dso = parseInt('0');
							}
							if(isNaN(pv)){
								var pv = parseInt('0');
							}
							if(isNaN(batt)){
								var batt = parseInt('0');
							}
							
							//console.log(batt,pv,dso);
							$.getJSON('data/dso.json.php?dso='+dso, function(data) {
								tot_value1 = batt + pv + data["power"];
								var z = tot_value1;
								var x = (new Date()).getTime(); // current time
								series1.addPoint([x, z], true, true);
							});
                            $.getJSON('data/lamp.json.php')
								.done(function(data) {//requested
								if(isNaN(unc)){
									tot_value = parseInt('0')+data["power"];
								}else{
									tot_value = unc+data["power"];
								}
								var y = tot_value;
								var x = (new Date()).getTime(); // current time
								series.addPoint([x, y], true, true);
                            	})
								.fail(function(data) {//requested
								if(isNaN(unc)){
									tot_value = parseInt('0');
								}else{
									tot_value = unc;
								}
								var y = tot_value;
								var x = (new Date()).getTime(); // current time
								series.addPoint([x, y], true, true);
								console.log("Requested Graph Error: NO DATA FROM 'data/lamp.json.php' ");
                            	})								
                        }, 60000);
                    }
                }
            },
            title: {
                text: 'Live Offered/Requested Power History'
            },
            xAxis: {
                type: 'datetime',
                tickPixelInterval: 30000,
                tickInterval: 360000
            },
            yAxis: [{
                title: {
                    text: 'W'
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            }],
            plotOptions: {
            spline: {
                lineWidth: 4,
                states:{
                    hover:{
                    lineWidth: 5
                    }
                },
                marker: {
                    enabled: false
                    }
                }
            },
            tooltip: {
                formatter: function () {
                    return '<b>' + this.series.name + '</b><br/>' +
                        Highcharts.dateFormat('Value at %H:%M:%S', this.x) + '<br/>' +
                        Highcharts.numberFormat(this.y, 2);
                }
            },
            legend: {
                enabled: true
            },
            credits: {
                enabled: false
            },
            exporting: {
                enabled: false
            },
            series: [{
                name: 'Requested Power',
                data: (function () {
					var unc = parseInt($("#unc").val());
                    if(isNaN(unc)){
						var unc = parseInt('0');
					}
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: unc
                        });
                    }
                    return data;
                }())
            },{
                name: 'Offered Power',
                data: (function () {

                    var dso = parseInt($("#dso").val());
					var pv = parseInt($("#pv").val());
					var batt = parseInt($("#batt").val());
					if(isNaN(dso)){
						var dso = parseInt('0');
					}
					if(isNaN(pv)){
						var pv = parseInt('0');
					}
					if(isNaN(batt)){
						var batt = parseInt('0');
					}
					
                    var data = [],
                        time = (new Date()).getTime(),
                        i;
					//console.log(batt,pv,dso);
					tot = batt+pv+dso
                    
                    var data = [],
                        time = (new Date()).getTime(),
                        i;

                    for (i = -59; i <= 0; i += 1) {
                        data.push({
                            x: time + i * 60000,
                            y: tot
                        });
                    }
                    return data;
                }())
            }
            ]
        });
}

