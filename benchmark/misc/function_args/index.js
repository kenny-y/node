// show the difference between calling a V8 binding C++ function
// relative to a comparable N-API C++ function,
// in various types and numbers of arguments.
// Reports n of calls per second.
'use strict';

const assert = require('assert');
const common = require('../../common.js');

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
                   'LargeObject', 'HugeArray',
                   '10Arguments', '100Arguments', '1000Arguments'];

let types = [];

['v8', 'napi'].forEach(function(type) {
  argsTypes.forEach(function(arg) {
    types.push(type + '-' + arg);
  });
});

function generateArgs(type) {
  let argsArray = [];
  let argType = type.split('-')[1];
  if (argType === 'String') {
    argsArray.push('The quick brown fox jumps over the lazy dog');
  } else if (argType === 'Number') {
    argsArray.push(3.14159165358964);
  } else if (argType === 'Object') {
    argsArray.push({
      map: 'add',
      operand: 10,
      data: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
      reduce: 'add',
    });
  } else if (argType === 'Array') {
    let arr = [];
    for (let i = 0; i < 1000; ++ i) {
      arr.push(Math.random() * 10e9);
    }
    argsArray.push(arr);
  } else if (argType === 'Typedarray') {
    let arr = new Uint32Array(1000);
    for (let i = 0; i < 1000; ++ i) {
      arr[i] = Math.random() * 4294967296;
    }
    argsArray.push(arr);
  }
  return argsArray;
}

const bench = common.createBenchmark(main, {
  type: types,
  n: [1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7]
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
