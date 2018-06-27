// show the difference between calling a V8 binding C++ function
// relative to a comparable N-API C++ function,
// in various types/numbers of arguments.
// Reports n of calls per second.
'use strict';

const common = require('../../common.js');

let v8;
let napi;

try {
  v8 = require('./build/Release/binding');
} catch (err) {
  // eslint-disable-next-line no-path-concat
  console.error(__filename + ': V8 Binding failed to load');
  process.exit(0);
}

try {
  napi = require('./build/Release/napi_binding');
} catch (err) {
  // eslint-disable-next-line no-path-concat
  console.error(__filename + ': NAPI-Binding failed to load');
  process.exit(0);
}

const argsTypes = ['String', 'Number', 'Object', 'Array', 'Typedarray',
                   '10Numbers', '100Numbers', '1000Numbers'];

const range = function (size) {
  return [...Array(size).keys()];
}

const generateArgs = (argType) => {
  let args = [];

  if (argType === 'String') {
    args.push('The quick brown fox jumps over the lazy dog');
  } else if (argType === 'LongString') {
    args.push(Buffer.alloc(32768, '42').toString());
  } else if (argType === 'Number') {
    args.push(Math.floor(314158964 * Math.random()));
  } else if (argType === 'Object') {
    args.push({
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
    args.push(arr);
  } else if (argType === 'Typedarray') {
    const arr = new Uint32Array(1000);
    for (let i = 0; i < 1000; ++i) {
      arr[i] = Math.random() * 4294967296;
    }
    args.push(arr);
  } else if (argType === '10Numbers') {
    args.push(10);
    for (let i in range(9)) {
      args = [...args, ...generateArgs('Number')];
    }
  } else if (argType === '100Numbers') {
    args.push(100);
    for (let i in range(99)) {
      args = [...args, ...generateArgs('Number')];
    }
  } else if (argType === '1000Numbers') {
    args.push(1000);
    for (let i in range(999)) {
      args = [...args, ...generateArgs('Number')];
    }
  }

  return args;
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
