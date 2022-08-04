
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


const irc = require('irc');


/**
 * @typedef {{ 
 * 	host: string, 
 * 	port: number,
 * 	password: string
 * }} InstanceConfig
 * 
 * @typedef {{
 * 	username: string,
 * 	nickname: string
 * }} InstanceUser
 */
class Instance {

	connected = false

	/**
	 * @param {InstanceConfig} serverConfig 
	 * @param {InstanceUser} firstUser
	 */
	constructor(serverConfig, firstUser) {
		if (!serverConfig)
			throw new Error('serverConfig is required')
		if (!firstUser)
			throw new Error('firstUser is required')
		this.client = new irc.Client(serverConfig.host, firstUser.nickname, {
			password: serverConfig.password,
			port: serverConfig.port,
			userName: firstUser.username,
			autoConnect: false
		})
	}

	connect() {
		return new Promise((resolve) => {
			this.client.connect(0, () => {
				this.connected = true
				resolve(this.connected)
			})
		})
	}

	destroy() {
		this.client.disconnect()
	}
}


/**
 * @type {InstanceConfig}
 */
const serverConfig = {
	host: "localhost",
	port: 10000,
	password: "06112001"
}

; (async () => {
	const instance = new Instance(serverConfig, {
		username: "Arthur",
		nickname: "arthur"
	})

	await instance.connect()

	instance.client.on('registered', () => {
		console.log('registered')
	})
	
	instance.client.on("raw", (info) => {
		console.log(info)
	})

	while(true) {
		await new Promise(resolve => setTimeout(resolve, 10))
		try {
			instance.client.send("JOIN", "#salut")
		} catch (e) {

		}
	}

})()
// instance.destroy()