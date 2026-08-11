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

  function setupDashboard() {
    const searchInput = document.getElementById("library-search");
    const cards = Array.from(document.querySelectorAll("[data-library-card]"));

    if (!searchInput || cards.length === 0) {
      return;
    }

    const filterButtons = Array.from(
      document.querySelectorAll("[data-filter-group][data-filter-value]")
    );
    const resetButtons = Array.from(
      document.querySelectorAll("#filter-reset, [data-reset-filters]")
    );
    const sections = Array.from(
      document.querySelectorAll("[data-result-section]")
    );
    const resultCount = document.getElementById("result-count");
    const resultLabel = document.getElementById("result-label");
    const emptyState = document.getElementById("dashboard-empty");

    const state = {
      kind: "all",
      status: "all",
      category: "all"
    };

    function normalize(value) {
      return String(value || "").trim().toLocaleLowerCase();
    }

    function setText(id, value) {
      const element = document.getElementById(id);
      if (element) {
        element.textContent = String(value);
      }
    }

    function updateStats() {
      const libraryCards = cards.filter((card) => card.dataset.kind === "library");
      const testCards = cards.filter((card) => card.dataset.kind === "verification");
      const verifiedCards = cards.filter((card) => card.dataset.status === "verified");
      const reviewCards = cards.filter((card) => card.dataset.status !== "verified");

      setText("stat-library-count", libraryCards.length);
      setText("stat-verified-count", verifiedCards.length);
      setText("stat-test-count", testCards.length);
      setText("stat-review-count", reviewCards.length);
    }

    function syncFilterButtons() {
      filterButtons.forEach((button) => {
        const group = button.dataset.filterGroup;
        const selected = state[group] === button.dataset.filterValue;
        button.setAttribute("aria-pressed", String(selected));
      });
    }

    function applyFilters() {
      const query = normalize(searchInput.value);
      let visibleTotal = 0;

      cards.forEach((card) => {
        const matchesQuery = !query || normalize(card.dataset.search).includes(query);
        const matchesKind = state.kind === "all" || card.dataset.kind === state.kind;
        const matchesStatus = state.status === "all" || card.dataset.status === state.status;
        const matchesCategory =
          state.category === "all" || card.dataset.category === state.category;
        const visible =
          matchesQuery && matchesKind && matchesStatus && matchesCategory;

        card.hidden = !visible;
        if (visible) {
          visibleTotal += 1;
        }
      });

      sections.forEach((section) => {
        const kind = section.dataset.resultSection;
        const visibleCards = section.querySelectorAll("[data-library-card]:not([hidden])");
        const countTarget = document.querySelector(`[data-visible-count="${kind}"]`);

        section.hidden = visibleCards.length === 0;
        if (countTarget) {
          countTarget.textContent = String(visibleCards.length);
        }
      });

      if (resultCount) {
        resultCount.textContent = String(visibleTotal);
      }
      if (resultLabel) {
        resultLabel.textContent = visibleTotal === 1 ? "item" : "items";
      }
      if (emptyState) {
        emptyState.hidden = visibleTotal !== 0;
      }
    }

    function resetFilters() {
      state.kind = "all";
      state.status = "all";
      state.category = "all";
      searchInput.value = "";
      syncFilterButtons();
      applyFilters();
    }

    filterButtons.forEach((button) => {
      button.addEventListener("click", function () {
        const group = button.dataset.filterGroup;
        const value = button.dataset.filterValue;

        state[group] = value;

        if (group === "category" && value !== "all") {
          state.kind = "library";
        }

        if (group === "kind" && value === "verification") {
          state.category = "all";
        }

        syncFilterButtons();
        applyFilters();
      });
    });

    resetButtons.forEach((button) => {
      button.addEventListener("click", resetFilters);
    });

    searchInput.addEventListener("input", applyFilters);

    document.addEventListener("keydown", function (event) {
      const activeTag = document.activeElement
        ? document.activeElement.tagName
        : "";
      const typing = /INPUT|TEXTAREA|SELECT/.test(activeTag);

      if (event.key === "/" && !typing) {
        event.preventDefault();
        searchInput.focus();
        searchInput.select();
      }

      if (event.key === "Escape" && (document.activeElement === searchInput || searchInput.value)) {
        searchInput.value = "";
        searchInput.blur();
        applyFilters();
      }
    });

    updateStats();
    syncFilterButtons();
    applyFilters();
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", setupDashboard);
  } else {
    setupDashboard();
  }
})();
