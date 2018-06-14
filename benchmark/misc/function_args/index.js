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

const argsTypes = ['String', 'Number', 'Object', 'Array', 'Typedarray',
                   'StringArray', 'ObjectArray', 'ArrayArray',
                   '10Arguments', '100Arguments', '1000Arguments'];

let types = [];

['v8', 'napi'].forEach(function(type) {
  argsTypes.forEach(function(arg) {
    types.push(type + '-' + arg);
  });
});

const bench = common.createBenchmark(main, {
  type: types,
  n: [1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6]
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
