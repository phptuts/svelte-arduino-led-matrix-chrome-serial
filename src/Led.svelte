<script>
  import { createEventDispatcher } from "svelte";
  export let isOn;
  export let ledNumber;
  const dispatch = createEventDispatcher();
  $: backgroundColor = isOn ? "red" : "white";
  function onChangeLed() {
    isOn = !isOn;
    dispatch("changeLed", { ledNumber, isOn });
  }
</script>

<div
  class="led"
  on:click={onChangeLed}
  style="background-color: {backgroundColor}" />
