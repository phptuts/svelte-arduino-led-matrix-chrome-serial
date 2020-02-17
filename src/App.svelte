<script>
  import Led from "./Led.svelte";
  import _ from "lodash";

  let leds = _.range(0, 8).map(() => _.range(0, 8).map(() => false));
  let writer;
  let showError;
  let errorMessage;
  const enc = new TextEncoder(); // always utf-8

  async function connectUSB() {
    showError = false;
    try {
      const port = await navigator.serial.requestPort();

      await port.open({
        baudrate: 115200
      });
      writer = port.writable.getWriter();
    } catch (e) {
      showError = true;
      console.error(e);
      errorMessage = e.message;
    }
  }

  function clearScreen() {
    leds = _.range(0, 8).map(() => _.range(0, 8).map(() => false));
  }

  function onChangeLed(e) {
    const { ledNumber, isOn } = e.detail;
    const rowNumber = Math.floor(ledNumber / 8);
    const columnNumber = ledNumber % 8;
    leds[rowNumber][columnNumber] = isOn;
    leds = [...leds];
  }
  let ledString = "";
  $: {
    ledString =
      _.clone(leds)
        .reverse()
        .reduce((prev, next, index) => {
          const startingString = index === 0 ? "" : ":";
          return (
            prev +
            startingString +
            next.map(isOn => (isOn ? "1" : "0")).join(":")
          );
        }, "") + "|";
    if (writer) {
      writer.write(enc.encode(ledString));
    }
  }
</script>

<style>
  .header {
    text-align: center;
  }
  .error {
    background: red;
    color: white;
    width: 400px;
    text-align: center;
    margin: 20px auto;
  }
  section {
    display: flex;
    height: 45px;
    width: 440px;
    margin: 10px auto;
  }
  div.message {
    border: black solid 1px;
    font-size: 0.9em;
    text-align: center;
    margin-top: 40px;
  }
  :global(div.led) {
    flex: 1;
    border: solid black 1px;
    height: 45px;
    border-radius: 22.5px;
    margin: 5px;
  }
</style>

<main class="container">
  {#if showError}
    <div class="row error">{errorMessage}</div>
  {/if}
  <div class="row header">
    <h1>Led Matrix Draw</h1>
    <button on:click={connectUSB} class="button-primary">Connect to USB</button>
    <button on:click={clearScreen} class="button-primary">Clear Screen</button>

  </div>

  {#each leds as rowLeds, row}
    <section>
      {#each rowLeds as isOn, col}
        <Led ledNumber={row * 8 + col} {isOn} on:changeLed={onChangeLed} />
      {/each}
    </section>
  {/each}
  <div class="row message">
    <h3>Message to USB</h3>
    <p>{ledString}</p>
  </div>
</main>
