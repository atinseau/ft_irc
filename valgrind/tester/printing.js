const { setBackground, bg, setFont, font, setForground, fg } = require('./colors')


/**
 * 
 * @param {string} name 
 * @param {Test[]} tests 
 */
 const printGroup = (name, tests) => {
	const formattedName = setBackground(name.toUpperCase(), bg.cyan)
	const formattedTests = setFont(tests.length + " test(s)", font.bright)
	console.log(`%s %s\n`, formattedName, formattedTests)
}

/**
 * 
 * @param {number} pass 
 * @param {number} fail 
 */
const printAtGroupEnd = (pass, fail) => {
	console.log(`\n %s %s\n`, 
		setForground(setFont(pass + " tests passed,", font.bright), fg.green),
		setForground(setFont(fail + " test failed", font.bright), fg.red)
	)
}

/**
 * 
 * @param {boolean | Error} err 
 * @param {any} ret 
 * @param {string[]} ouput
 * @returns {boolean}
 */
const printTestResult = (err, ret, output) => {
	let pass = false

	if (err) {
		console.log(setBackground("FAIL", bg.red))
		console.log("%s, %s",
			setForground(`Test fail because an error was occured`, fg.magenta),
			setForground(err === true ? ret : err.message, fg.red)
		)
	}
	else {
		console.log(setBackground("PASS", bg.green))
		pass = true
	}
	for (const line of output)
		console.log(line)
	return pass
}

/**
 * @template {Function} T
 * @param {T} fn
 * @returns {[boolean | string, ReturnType<T>, string[]]}
 */
async function preventLog (fn) {
	const buffer = []
	const log = console.log
	let ret = null
	let err = false

	console.log = (...args) => buffer.push(...args)
	try {
		ret = await fn.call(this.context, this)
		if (!ret)
			err = true
	} catch (e) {
		err = e
	}
	console.log = log
	return [err, ret, buffer]
}

module.exports = {
	printGroup,
	printAtGroupEnd,
	preventLog,
	printTestResult
}