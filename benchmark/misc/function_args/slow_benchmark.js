// show the difference between calling a V8 binding C++ function
// relative to a comparable N-API C++ function,
// in various types and numbers of arguments.
// Reports n of calls per second.
'use strict';

const assert = require('assert');
const common = require('../../common.js');
const {generateArgs} = require('./generate_args.js');

let v8;
let napi;

try {
  v8 = require('./build/Release/binding');
} catch (er) {
  console.error('misc/function_args/index.js Binding failed to load');
  process.exit(0);
}

try {
  napi = require('./build/Release/napi_binding');
} catch (er) {
  console.error('misc/function_args/index.js NAPI-Binding failed to load');
  process.exit(0);
}

const slowArgsTypes = ['LargeArray', 'LargeObject', 'HugeArray', 'LongString'];

let slowTypes = [];

['v8', 'napi'].forEach(function(type) {
  slowArgsTypes.forEach(function(arg) {
    slowTypes.push(type + '-' + arg);
  });
});

const bench = common.createBenchmark(main, {
  type: slowTypes,
  n: [1, 5, 10]
});

function main({ n, type }) {
  const bindings = type.split('-')[0] === 'v8' ? v8 : napi;
  const methodName = 'callWith' + type.split('-')[1]
  const fn = bindings[methodName];

  if (fn) {
    let args = generateArgs(type);

    bench.start();
    for (var i = 0; i < n; i++) {
      fn.apply(null, args);
    }
    bench.end(n);
  }
}
