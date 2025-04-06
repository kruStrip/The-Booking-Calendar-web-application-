import React from "react";
import { createRoot } from "react-dom/client";
import { A } from "./screens/A";
import "./tailwind.css";

const root = createRoot(document.getElementById("app") as HTMLElement);
root.render(
  <React.StrictMode>
    <A />
  </React.StrictMode>
); 