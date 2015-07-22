/*! ineaticsDashboard.js - functions for rendering statistics - Copyright (C) 2015 - INAETICS */
"use strict";

const interval = 1000 // ms

function renderUtilisation(utilisation) {
	document.getElementById('utilisation').textContent = utilisation.name + " = " + utilisation.value + " " + utilisation.unit
}

function getAndRenderUtilisation() {
	getJSON('/utilisation', renderUtilisation, interval)
}

function renderSystemStats(stats) {
	var tbody = document.getElementById('sysStats')

	while (tbody.firstChild) {
		tbody.removeChild(tbody.firstChild);
	}

	for (var key in stats) {
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
	getJSON('/systemStats', renderSystemStats, interval)
}

function postAndUpdateUtilisation(val) {
	document.getElementById('slider-val').value = val
	
	var data = new FormData()
	data.append('value', val)

	postJSON('/utilisation', data)
}

function getJSON(url, callback, ival) {
	var xhr = new XMLHttpRequest()
	xhr.open('GET', url, true)
	xhr.responseType = 'json'
	xhr.onload = function(e) {
		try {
			if (this.status != 200) {
				console.log("failed to obtain " + url)
			} else {
				callback(this.response)
			}
		} finally {
			if (ival > 0) {
				setTimeout(function() { getJSON(url, callback, ival) }, ival)
			}
		}
	}
	xhr.send();
}

function postJSON(url, data) {
	var xhr = new XMLHttpRequest()
	xhr.open('POST', url, true)
	xhr.send(data)
}

window.onload = function() {
	setTimeout(getAndRenderUtilisation, interval)
	setTimeout(getAndRenderSystemStats, interval)
}
