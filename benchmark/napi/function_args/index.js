// show the difference between calling a V8 binding C++ function
// relative to a comparable N-API C++ function,
// in various types and numbers of arguments.
// Reports n of calls per second.
'use strict';

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
                   '10Arguments', '100Arguments', '1000Arguments'];

const generateArgs = (argType) => {
  let argsArray = [];

  if (argType === 'String') {
    argsArray.push('The quick brown fox jumps over the lazy dog');
  } else if (argType === 'LongString') {
    argsArray.push(Buffer.alloc(65536, '42').toString());
  } else if (argType === 'Number') {
    argsArray.push(31415916);
  } else if (argType === 'Object') {
    argsArray.push({
      map: 'add',
      operand: 10,
      data: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
      reduce: 'add',
    });
  } else if (argType === 'Array') {
    const arr = [];
    for (let i = 0; i < 50; ++i) {
      arr.push(Math.random() * 10e9);
    }
    argsArray.push(arr);
  } else if (argType === 'Typedarray') {
    const arr = new Uint32Array(1000);
    for (let i = 0; i < 1000; ++i) {
      arr[i] = Math.random() * 4294967296;
    }
    argsArray.push(arr);
  } else if (argType === '10Arguments') {
    argsArray.push(10);
    // eslint-disable-next-line array-callback-return
    Array(10 - 1).fill().map((_, i) => {
      argsArray = [...argsArray, ...generateArgs('Number')];
    });
  } else if (argType === '100Arguments') {
    argsArray.push(100);
    // eslint-disable-next-line array-callback-return
    Array(100 - 1).fill().map((_, i) => {
      argsArray = [...argsArray, ...generateArgs('Number')];
    });
  } else if (argType === '1000Arguments') {
    argsArray.push(1000);
    // eslint-disable-next-line array-callback-return
    Array(1000 - 1).fill().map((_, i) => {
      argsArray = [...argsArray, ...generateArgs('Number')];
    });
  }

  return argsArray;
};

const getArgs = (type) => {
  return generateArgs(type.split('-')[1]);
};

const benchTypes = [];

argsTypes.forEach((arg) => {
  ['v8', 'napi'].forEach((type) => {
    benchTypes.push(type + '-' + arg);
  });
});

const bench = common.createBenchmark(main, {
  type: benchTypes,
  n: [1, 1e1, 1e2, 1e3, 1e4, 1e5],
});

function main({ n, type }) {
  const bindings = type.split('-')[0] === 'v8' ? v8 : napi;
  const methodName = 'callWith' + type.split('-')[1];
  const fn = bindings[methodName];

  if (fn) {
    const args = getArgs(type);

    bench.start();
    for (var i = 0; i < n; i++) {
      fn.apply(null, args);
    }
    bench.end(n);
  }
}
