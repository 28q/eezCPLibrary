(function () {
  "use strict";

  function setupCodeViewer() {
    const panel = document.getElementById("document-code-panel");
    if (!panel) {
      return;
    }

    const tabButtons = Array.from(panel.querySelectorAll("[data-code-tab]"));
    const views = Array.from(panel.querySelectorAll("[data-code-view]"));
    const copyButton = document.getElementById("copy-code");
    const downloadButton = document.getElementById("download-code");
    const wrapButton = document.getElementById("toggle-wrap");
    const expandButton = document.getElementById("toggle-expand");
    const fileLabel = document.getElementById("active-code-file");
    const lineLabel = document.getElementById("active-code-lines");
    const toast = document.getElementById("site-toast");
    const includeButtons = Array.from(document.querySelectorAll("[data-copy-text]"));

    let activeKind = "source";
    let toastTimer = 0;

    function showToast(message) {
      if (!toast) {
        return;
      }

      window.clearTimeout(toastTimer);
      toast.textContent = message;
      toast.classList.add("is-visible");
      toastTimer = window.setTimeout(function () {
        toast.classList.remove("is-visible");
      }, 1800);
    }

    async function copyText(text) {
      try {
        await navigator.clipboard.writeText(text);
      } catch (_) {
        const textarea = document.createElement("textarea");
        textarea.value = text;
        textarea.style.position = "fixed";
        textarea.style.opacity = "0";
        document.body.appendChild(textarea);
        textarea.select();
        document.execCommand("copy");
        textarea.remove();
      }
    }

    function activeView() {
      return views.find((view) => view.dataset.codeView === activeKind);
    }

    function codeFromView(view) {
      if (!view) {
        return "";
      }

      const codeElement = view.querySelector("code") || view.querySelector("pre");
      if (!codeElement) {
        return "";
      }

      return codeElement.textContent
        .replace(/^\n/, "")
        .replace(/\n$/, "");
    }

    function bundledFileName(sourceName) {
      const dot = sourceName.lastIndexOf(".");
      if (dot <= 0) {
        return sourceName + ".bundle.cpp";
      }

      return sourceName.slice(0, dot) + ".bundle" + sourceName.slice(dot);
    }

    function activeFileName() {
      const sourceName = panel.dataset.sourceName || "library.cpp";
      return activeKind === "bundle" ? bundledFileName(sourceName) : sourceName;
    }

    function updateCodeMeta() {
      const code = codeFromView(activeView());
      const lines = code ? code.split("\n").length : 0;

      if (fileLabel) {
        fileLabel.textContent = activeFileName();
      }
      if (lineLabel) {
        lineLabel.textContent = lines + (lines === 1 ? " line" : " lines");
      }
    }

    function selectCode(kind) {
      const button = tabButtons.find((item) => item.dataset.codeTab === kind);
      if (!button || button.disabled) {
        return;
      }

      activeKind = kind;

      tabButtons.forEach((item) => {
        const active = item.dataset.codeTab === activeKind;
        item.classList.toggle("is-active", active);
        item.setAttribute("aria-selected", String(active));
      });

      views.forEach((view) => {
        const active = view.dataset.codeView === activeKind;
        view.hidden = !active;
        view.classList.toggle("is-active", active);
      });

      updateCodeMeta();
    }

    async function copyActiveCode() {
      const code = codeFromView(activeView());
      if (!code) {
        showToast("コピーするコードがありません");
        return;
      }

      await copyText(code);
      showToast(activeKind === "bundle" ? "Bundleをコピーしました" : "コードをコピーしました");

      if (copyButton) {
        const original = copyButton.textContent;
        copyButton.textContent = "Copied!";
        window.setTimeout(function () {
          copyButton.textContent = original;
        }, 1200);
      }
    }

    function downloadActiveCode() {
      const code = codeFromView(activeView());
      if (!code) {
        showToast("ダウンロードするコードがありません");
        return;
      }

      const fileName = activeFileName();
      const url = URL.createObjectURL(new Blob([code + "\n"], { type: "text/plain;charset=utf-8" }));
      const anchor = document.createElement("a");
      anchor.href = url;
      anchor.download = fileName;
      document.body.appendChild(anchor);
      anchor.click();
      anchor.remove();
      window.setTimeout(function () {
        URL.revokeObjectURL(url);
      }, 1000);
      showToast(fileName + "をダウンロードしました");
    }

    function toggleWrap() {
      const wrapped = panel.classList.toggle("is-wrapped");
      if (wrapButton) {
        wrapButton.setAttribute("aria-pressed", String(wrapped));
        wrapButton.textContent = wrapped ? "Unwrap" : "Wrap";
      }
    }

    function setExpanded(expanded) {
      panel.classList.toggle("is-expanded", expanded);
      document.body.classList.toggle("code-expanded", expanded);

      if (expandButton) {
        expandButton.setAttribute("aria-pressed", String(expanded));
        expandButton.textContent = expanded ? "Close" : "Expand";
      }
    }

    tabButtons.forEach((button) => {
      button.addEventListener("click", function () {
        selectCode(button.dataset.codeTab);
      });
    });

    if (copyButton) {
      copyButton.addEventListener("click", copyActiveCode);
    }
    if (downloadButton) {
      downloadButton.addEventListener("click", downloadActiveCode);
    }
    if (wrapButton) {
      wrapButton.addEventListener("click", toggleWrap);
    }
    if (expandButton) {
      expandButton.addEventListener("click", function () {
        setExpanded(!panel.classList.contains("is-expanded"));
      });
    }

    includeButtons.forEach((button) => {
      button.addEventListener("click", async function () {
        await copyText(button.dataset.copyText || "");
        showToast("include文をコピーしました");
      });
    });

    document.addEventListener("keydown", function (event) {
      const activeTag = document.activeElement ? document.activeElement.tagName : "";
      const typing = /INPUT|TEXTAREA|SELECT/.test(activeTag);

      if (event.key === "Escape" && panel.classList.contains("is-expanded")) {
        setExpanded(false);
        return;
      }

      if (typing || event.ctrlKey || event.metaKey || event.altKey) {
        return;
      }

      if (event.key.toLocaleLowerCase() === "b") {
        const next = activeKind === "source" ? "bundle" : "source";
        selectCode(next);
      } else if (event.key.toLocaleLowerCase() === "c") {
        copyActiveCode();
      } else if (event.key.toLocaleLowerCase() === "w") {
        toggleWrap();
      }
    });

    selectCode("source");
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", setupCodeViewer);
  } else {
    setupCodeViewer();
  }
})();
