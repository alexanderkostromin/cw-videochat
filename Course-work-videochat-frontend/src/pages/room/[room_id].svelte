<script>
  import { onMount } from  'svelte';
  import { params, goto } from '@roxi/routify';
  import {backendUrl, sendData} from "../../_api";
  import { Diamonds } from 'svelte-loading-spinners';
  import { fly } from 'svelte/transition';

  let help = false;

  const constraints = {
    video: true,
    audio: true
  }

  const config = {
    configuration: {
      offerToReceiveAudio: true,
      offerToReceiveVideo: true
    },
    iceServers: [
      {url: 'stun:stun1.l.google.com:19302'},
      {url: 'stun:stun.dcalling.de:3478'},
      {url: 'stun:stun.schlund.de'},
      {url: 'stun:stun.voxgratia.org'}
    ]
  }

  let creator = false;
  const peerConnection = new RTCPeerConnection(config);

  let localStream;
  let remoteStream = new MediaStream();

  onMount(() => {
    navigator.mediaDevices.getUserMedia(constraints)
        .then(stream => {
          console.log('Got MediaStream:', stream);
          const videoElement = document.querySelector('#local-video');
          videoElement.srcObject = stream;
          stream.getTracks().forEach(track => {
            peerConnection.addTrack(track, stream);
          })
          creator = localStorage.getItem('creator') == $params.room_id;
          startSession();
        })
        .catch(error => {
          alert("Не удается подключиться к камере!")
          console.error('Error accessing media devices.', error);
        });
    const remoteVideo = document.querySelector('#remote-video');
    remoteVideo.srcObject = remoteStream;
  })


  async function startSession() {
    if (creator) {
      creatorSession().then(()=> {
        console.log(peerConnection);
      })

    }
    else {
      userSession().then(() => {
        console.log(peerConnection);
      })

    }

  }

  async function creatorSession() {
    const res = await setOffer();
    if (!res.error) {
      const answer = JSON.parse(await waitFor('answer'));
      const remoteDesc = new RTCSessionDescription(answer);
      await peerConnection.setRemoteDescription(remoteDesc);

      const iceCandidate = JSON.parse(await waitFor('ice_candidate_answer'));
      await peerConnection.addIceCandidate(iceCandidate);
      console.log('add_ice_candidate')
    } else {
      await creatorSession();
    }
  }

  function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  async function waitFor(field) {
    return getRoom()
        .then(async json => {
          if (json[field])
            return json[field];
          else {
            await sleep(3000);
            return waitFor(field);
          }
        })
  }

  async function setOffer() {
    const offer = await peerConnection.createOffer();
    console.log(offer)
    await peerConnection.setLocalDescription(offer);
    console.log('set_offer')
    return await sendData('set_offer', 'PUT', {id: parseInt($params.room_id), offer: JSON.stringify(offer)});
  }

  async function userSession() {
    getRoom().then(async json => {
      if (json.offer) {
        await setAnswer(json.offer);

        const iceCandidate = JSON.parse(await waitFor('ice_candidate_offer'));
        await peerConnection.addIceCandidate(iceCandidate);
        console.log('add_ice_candidate');
      }
      else
        await userSession();
    })
  }

  async function setAnswer(offer) {
    offer = JSON.parse(offer);
    console.log(offer);
    await peerConnection.setRemoteDescription(new RTCSessionDescription(offer));
    const answer = await peerConnection.createAnswer();
    await peerConnection.setLocalDescription(answer);
    await sendData('set_answer', 'PUT', {id: parseInt($params.room_id), answer: JSON.stringify(answer)})
    console.log('set_answer')
  }

  async function getRoom() {
    const res = await fetch(backendUrl+'get_room?id='+$params.room_id);
    return await res.json();
  }

  peerConnection.onicecandidate = async function (event) {
    console.log('ICECAND')
    if (event.candidate)
      await sendData('set_ice_candidate', 'PUT',
          {id: parseInt($params.room_id), creator, ice_candidate: JSON.stringify(event.candidate)})
  }

  peerConnection.addEventListener('connectionstatechange', event => {
    if (peerConnection.connectionState === 'connected') {
      console.log('connected')
    }
  });

  peerConnection.onicecandidateerror = function (error) {
    console.log(error);
  }

  peerConnection.ontrack = function (event) {
    remoteStream.addTrack(event.track, remoteStream);
  }

  async function disconnect() {
    peerConnection.close();
    $goto('/');
  }
</script>
<div id="component">
  <div style="">
    <Diamonds size="250" duration="2s"/>
  </div>
  <video id="local-video" autoplay playsinline/>
  <video id="remote-video" autoplay playsinline/>
</div>
<div id="controls">
  <div id="add-user">
    {#if help}
      <div id="help" transition:fly="{{ x: -200, duration: 300 }}">
        <p>Ссылка для входа в конференцию:</p>
        <p id="invite">{document.location.href}</p>
      </div>
    {/if}
    <img src="/add-user.png" on:click={() => help = !help}/>
  </div>

  <div id="disconnect" on:click={disconnect}>
    <img src="/phone.png"/>
  </div>
</div>

<style>
  #component {
    position: relative;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: #333333;
    display: flex;
    justify-content: center;
    align-items: center;
    z-index: 1;
  }
  #remote-video {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    z-index: 10;
  }

  #local-video {
    position: absolute;
    width: 200px;
    bottom: 0;
    right: 0;
    z-index: 11;
    border-top-left-radius: 10px;
  }

  #controls {
    position: absolute;
    bottom: 0;
    left: 0;
    width: 130px;
    height: 55px;
    border-top-right-radius: 10px;
    background-color: #999999;
    /*opacity: 0.5;*/
    z-index: 12;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  #controls div {
    margin: 0 10px;
  }

  #disconnect {
    width: 40px;
    height: 40px;
  }

  #disconnect img {
    width: 40px;
    height: 40px;
    cursor: pointer;
  }

  #add-user {
    background-color: #074EF2;
    border-radius: 20px;
    width: 40px;
    height: 40px;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  #help {
    position: absolute;
    top: -120px;
    left: -10px;
    background-color: #999999;
    padding: 0 20px;
    border-top-right-radius: 10px;
    border-bottom-right-radius: 10px;
  }

  #help p {
    margin: 10px 0;
  }

  #invite {
    border-radius: 2px;
    border: 1px solid #333333;
    padding: 5px 10px;
  }

  #add-user img {
    width: 60%;
    cursor: pointer;
  }
</style>