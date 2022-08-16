
class Reply {
	#host
	constructor(/** @type {string} */raw) {
		const host = raw.substring(1, raw.substring(1).indexOf(':'))
		this.message = raw.substring((host.length + 1) + 2).replace('\r', '')
		this.#host = host.split(' ')
	}
	get hostname() {
		return this.#host[0]
	}
	get code() {
		return this.#host[1]
	}
	get username() {
		return this.#host[2]
	}

	toJSON() {
		return {
			hostname: this.hostname,
			code: this.code,
			username: this.username,
			message: this.message
		}
	}
}

/**
 * 
 * @param {string[]} str 
 */
const format = function (str) {
	return (str[0].substring(1, str[0].length - 1))
		.replace(new RegExp('\\t', 'g'), '')
		.split('\n')
		.filter(line => line.length);
}

const child_process = require('node:child_process')

const irc = child_process.exec("nc -C localhost 10000")
