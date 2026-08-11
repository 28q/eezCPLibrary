(function () {
  "use strict";

  const root = document.documentElement;
  const themeButton = document.getElementById("theme-toggle");
  const themeColor = document.querySelector('meta[name="theme-color"]');

  function applyTheme(theme) {
    root.dataset.theme = theme;

    if (themeColor) {
      themeColor.content = theme === "dark" ? "#10120f" : "#f2f1eb";
    }

    if (themeButton) {
      themeButton.textContent = theme === "dark" ? "☀" : "◐";
      themeButton.setAttribute(
        "aria-label",
        theme === "dark"
          ? "ライトテーマに切り替える"
          : "ダークテーマに切り替える"
      );
      themeButton.title =
        theme === "dark"
          ? "ライトテーマに切り替える"
          : "ダークテーマに切り替える";
    }
  }

  function currentTheme() {
    if (root.dataset.theme === "dark" || root.dataset.theme === "light") {
      return root.dataset.theme;
    }

    return window.matchMedia("(prefers-color-scheme: dark)").matches
      ? "dark"
      : "light";
  }

  applyTheme(currentTheme());

  if (themeButton) {
    themeButton.addEventListener("click", function () {
      const nextTheme = currentTheme() === "dark" ? "light" : "dark";
      applyTheme(nextTheme);

      try {
        localStorage.setItem("eezcp-library-theme", nextTheme);
      } catch (_) {
        // localStorageが使えなくてもテーマ切替自体は続行する
      }
    });
  }
})();
