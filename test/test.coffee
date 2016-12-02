should = require 'should'

Error.stackTraceLimit = Infinity

# Make sure w'ere not using RegExp by mistake
RegExp_ = RegExp
RegExp = undefined

RegExp2 = require '..'

it 'should create a RegExp2 object successfully', ->
	RegExp2 'abcd'
	RegExp2 'abcd', 'g'
	RegExp2 'abcd', 'gmi'
	RegExp2 'abcd', 'gi'
	RegExp2 'abcd', 'ig'
	RegExp2 'abcd', 'img'
	RegExp2 'abcd', 'i'
	RegExp2 'abcdee1234(abcd)'

	new RegExp2 'abcd'
	new RegExp2 'abcd', 'g'
	new RegExp2 'abcd', 'gmi'
	new RegExp2 'abcd', 'gi'
	new RegExp2 'abcd', 'ig'
	new RegExp2 'abcd', 'img'
	new RegExp2 'abcd', 'i'
	new RegExp2 'abcdee1234(abcd)'

it 'should properly format a toString() result', ->
	RegExp2('abcd').toString().should.equal '/abcd/'
	RegExp2('abcd(efg)', 'g').toString().should.equal '/abcd(efg)/g'

it 'should fail on badly formatted regexes', ->
	(-> RegExp2 '*').should.throw 'Invalid regular expression /*/: One of *?+{ was not preceded by a valid regular expression.'
