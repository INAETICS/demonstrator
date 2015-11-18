/*! ineaticsDashboard.js - functions for rendering statistics - Copyright (C) 2015 - INAETICS */
"use strict";

var interval = 1000 // ms

function renderUtilisation(utilisation) {
	document.getElementById('utilisation').textContent = utilisation.name + " = " + utilisation.value + " " + utilisation.unit
}

function renderGraph(canvasId, stats) {
	var data = getData(stats)
	var opts = getChartOpts(stats)

	var statCanvas = document.getElementById(canvasId)
	var chartCtx = statCanvas.getContext('2d');

	if (statCanvas.hasAttribute("hasData")) {
		updateChart(chartCtx, data, opts, false /* animation */, false /* runanimationcompletefunction */);
	} else {
		var chart = new Chart(chartCtx);
		chart.Line(data, opts);
		// Mark the canvas as having data...
		statCanvas.setAttribute("hasData", "true")
	}
}

function renderUtilisationGraph(stats) {
	
	if (!stats) return
	
	var queueIdx = stats[0].name == 'queue' ? 0 : 1

	renderGraph('queue-utilisation', stats[queueIdx])
	renderGraph('datastore-utilisation', stats[1 - queueIdx])
}

function getAndRenderUtilisation() {
	getJSON('/coordinator/utilisation', renderUtilisation, interval)
	getJSON('/coordinator/statistics?queue,data-store', renderUtilisationGraph, interval)
}

function renderSystemStats(stats) {
	var tbody = document.getElementById('sysStats')

	while (tbody.firstChild) {
		tbody.removeChild(tbody.firstChild);
	}

	for (var key in stats) {
		if (key === 'productionRate') {
			var val = stats[key]
			document.getElementById('slider-val').value = val
			document.getElementById('slider').value = val
		}
		if (stats.hasOwnProperty(key)) {
			var tr = tbody.insertRow(0);

			var th = document.createElement('th')
			th.textContent = key.capitalizeFirstLetter()
			var td = document.createElement('td')
			td.textContent = stats[key]

			tr.insertCell(0).appendChild(th)
			tr.insertCell(1).appendChild(td)
			tbody.appendChild(tr)
		}
	}
}

function getAndRenderSystemStats() {
	getJSON('/coordinator/systemStats', renderSystemStats, interval)
}

function renderClusterInfo(stats) {
	var clusterInfo = document.getElementById('clusterInfo')
	
	while (clusterInfo.firstChild) {
		clusterInfo.removeChild(clusterInfo.firstChild);
	}
	
	if (stats.length === 0) {
		clusterInfo.innerText = 'no info available';
		return;
	}

	var table = document.createElement('table');
	table.className = 'table table-condensed';

	var headerRow = document.createElement('tr');

	var headers = ['Name', 'CPU', 'Memory'];
	for (var i = 0; i < headers.length; i++ ) {
		var header = document.createElement('th');
		header.textContent = headers[i];
		headerRow.appendChild(header);
	}

	table.appendChild(headerRow);

	for (var i = 0; i < stats.length; i++) {
		var host = stats[i];
		
		var hostRow = document.createElement('tr');
		hostRow.className = 'info';
		
		var hostName = document.createElement('td');
		hostName.textContent = host['ipAddress'];
		hostRow.appendChild(hostName);

		table.appendChild(hostRow);
		
		var containers = host['containerList'];
		for (var j = 0; j < containers.length; j++) {
			var container = containers[j];

			var name = '\u00a0\u00a0\u00a0\u00a0\u00a0' + container['name'].substring(0, container['name'].indexOf('.'));
			var cpu = container['cpuUsage'];
			var memory = parseInt(container['usedMem'] / 1000000) + "MB";

			var data  = [name, cpu, memory];
			
			var row = document.createElement('tr');
			for (var k = 0; k < data.length; k++) {
				var td = document.createElement('td');
				td.textContent = data[k]; 
				row.appendChild(td);
			}
			table.appendChild(row);

		}

		var row = document.createElement('tr');
		var td = document.createElement('td');
		td.textContent = '\u00a0';
		row.appendChild(td);
		table.appendChild(row);

	}

	clusterInfo.appendChild(table);

}

function getAndRenderClusterInfo() {
	getJSON('/coordinator/clusterInfo', renderClusterInfo, interval)
}

function postAndUpdateUtilisation(val) {
	document.getElementById('slider-val').value = val
	postJSON('/coordinator/utilisation', 'value=' + val)
}

function scaleProducer(el, val) {
	postJSON('/coordinator/scaleProducer', 'value=' + val)
	el.blur();
}

window.onload = function() {
	setTimeout(getAndRenderUtilisation, interval)
	setTimeout(getAndRenderSystemStats, interval)
	setTimeout(getAndRenderClusterInfo, interval)
}
