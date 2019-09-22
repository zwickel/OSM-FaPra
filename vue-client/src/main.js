import Vue from "vue";
import App from "./App.vue";
import store from "./store/store";
import vuetify from "./plugins/vuetify";

Vue.config.productionTip = false;

window.vueDevMode = process.env.NODE_ENV;
window.serverUrl = location.protocol + "//" + location.host + location.pathname;

if (window.vueDevMode === "development") {
  window.serverUrl = location.protocol + "//localhost:8091/";
}

new Vue({
  store,
  vuetify,
  render: h => h(App)
}).$mount("#app");
