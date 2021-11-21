import {params} from "@roxi/routify";

export let backendUrl = 'http://localhost:4000/'

export async function sendData(path, method, body) {
  return fetch(backendUrl+path, {
    method,
    headers: {
      'Content-Type': 'application/json',
      'Accept': 'application/json'
    },
    body: JSON.stringify(body)
  }).then(res => res.json())
      .then(json => {
    return json;
  }).catch(error => {
    console.log(error);
    return {error}
      })
}
