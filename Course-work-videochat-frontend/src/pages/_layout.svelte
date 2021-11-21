<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Comfortaa&display=swap" rel="stylesheet">

<svelte:head>
  <title>Videochat</title>
</svelte:head>

<div id="window">
  <slot />
</div>

<style>
  #window {
    position: absolute;
    width: 100%;
    height: 100%;
    background-color: #333333;
  }
</style>