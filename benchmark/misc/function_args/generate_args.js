'use strict';

function generateArgs(type) {
  let argsArray = [];
  let argType = type.split('-')[1];
  if (argType === 'String') {
    argsArray.push('The quick brown fox jumps over the lazy dog');
  } else if (argType === 'LongString') {
    argsArray.push(Buffer.alloc(65536, '42').toString());
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
    for (let i = 0; i < 50; ++ i) {
      arr.push(Math.random() * 10e9);
    }
    argsArray.push(arr);
  } else if (argType === 'LargeArray') {
    let arr = [];
    for (let i = 0; i < 1000; ++ i) {
      arr.push(Math.random() * 10e9);
    }
    argsArray.push(arr);
  } else if (argType === 'HugeArray') {
    let arr = [];
    for (let i = 0; i < 1e7; ++ i) {
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

module.exports = {
  generateArgs: generateArgs,
};
