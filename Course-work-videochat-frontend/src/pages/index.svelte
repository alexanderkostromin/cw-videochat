<script>
import { goto, params } from "@roxi/routify"
import {backendUrl } from "../_api";

let id;

async function createRoom() {
  fetch(backendUrl+'create_room', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'Accept': 'application/json'
    }, body: JSON.stringify({})
  }).then(res => res.json())
      .then(json => {
    localStorage.setItem('creator', json.id);
    $goto('/room/'+json.id)
  })
}
</script>

<main>
  <div id="form">
<!--    <input placeholder="Введите id комнаты" bind:value={id}>-->
<!--    <button on:click={() => $goto('/room/'+id)}>Войти</button>-->
<!--    ИЛИ-->
    <button on:click={createRoom}>Создать новую комнату</button>
  </div>
</main>

<style>
  main {
    position: absolute;
    top: 0;
    width: 100%;
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  #form {
    width: 600px;
    display: flex;
    flex-direction: column;
    text-align: center;
    margin: auto;
  }

  button {
    cursor: pointer;
    background-color: #FF3E00;
    border: 0;
    border-radius: 20px;
    color: #FFFFFF;
    width: 100%;
    padding: 40px;
    font-size: 35px;
  }
</style>