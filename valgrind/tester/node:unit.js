const { setBackground, bg, setFont, font } = require('./colors')
const { printGroup, printAtGroupEnd, preventLog, printTestResult } = require('./printing')
const process = require('process')

let context = []

/**
 * 
 * @param {Test} test
 * @returns {[boolean | string, any, string[]]}
 */
async function executeTest (test) {
	return await preventLog.call(this, test.f)
}




/**
 * 
 * @param {string} name 
 * @param {Function} f 
 */
async function describeBind (name, f) {

	f.call(this)
	printGroup(name, this.tests)

	if (this.beforeAll)
		await this.beforeAll.call(this.context, this)

	let pass = 0
	let fail = 0

	for (const test of this.tests) {
		process.stdout.write(setFont(setBackground(" " + test.name + " ", bg.yellow), font.bright) + "  ")
		const [err, ret, output] = await executeTest.call(this, test)
		printTestResult(err, ret, output) ? pass++ : fail++
	}
	printAtGroupEnd(pass, fail)

	if (this.afterAll)
		await this.afterAll.call(this.context, this)
	process.stdout.write('\n')
}

const describe = async (name, fn) => {
	if (context.length > 0)
		await context[context.length - 1].pending
	const ctx = context.push({
		tests: [],
		isContext: true,
		id: context.length,
		context: {}
	})
	context[ctx - 1].pending = describeBind.bind(context[ctx - 1], name, fn)()
}

/**
 * 
 * @param  {[name: string, f: Function]} args 
 * @returns 
 */
function it (...args) {
	return (function (name, f) {
		if (!this.isContext)
			throw new Error("it() must be called in a describe() context")
		this.tests.push({ name, f })
	}).bind(context[context.length - 1], ...args)()
}


/**
 * 
 * @param  {[f: Function]} args 
 * @returns 
 */
 function beforeAll (...args) {
	return (function (f) {
		if (!this.isContext)
			throw new Error("beforeAll() must be called in a describe() context")
			this.beforeAll = f
	}).bind(context[context.length - 1], ...args)()
}

/**
 * 
 * @param  {[f: Function]} args 
 * @returns 
 */
 function afterAll (...args) {
	return (function (f) {
		if (!this.isContext)
			throw new Error("afterAll() must be called in a describe() context")
			this.afterAll = f
	}).bind(context[context.length - 1], ...args)()
}


module.exports = {
	describe,
	it,
	beforeAll,
	afterAll
}