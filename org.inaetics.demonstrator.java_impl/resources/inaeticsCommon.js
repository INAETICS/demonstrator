/*! ineaticsCommon.js - shared functions - Copyright (C) 2015 - INAETICS */

/*! getJSON - recursively returns the JSON result when querying an URL.
 * inputs: url, the URL to retrieve;
 *         callback, the function to call when a JSON result is 
 *           obtained. The function is given the JSON response;
 *         ival, the interval in milliseconds.
 * output: none. 
 */
function getJSON(url, callback, ival) {
	var xhr = new XMLHttpRequest();
	xhr.open('GET', url, true);
	xhr.responseType = 'json';
	xhr.onload = function(e) {
		try {
			if (this.status != 200) {
				console.log("failed to obtain " + url);
			} else {
				callback(this.response)
			}
		} finally {
			if (ival > 0) {
				setTimeout(function() {
					getJSON(url, callback, ival)
				}, ival)
			}
		}
	}
	xhr.send();
}

/*! postJSON - posts (form) data to an URL.
 * inputs: url, the URL to post;
 *         data, the FormData object to post.
 */
function postJSON(url, data) {
	var xhr = new XMLHttpRequest()
	xhr.open('POST', url, true)
	xhr.send(data)
}

String.prototype.capitalizeFirstLetter = function() {
    return this.charAt(0).toUpperCase() + this.slice(1);
}
