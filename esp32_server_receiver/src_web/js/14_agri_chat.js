// ================= 14_AGRI_CHAT.JS: INTERACTIVE AGRONOMIST CHATBOT ================= //
window.agronomyChatHistory = window.agronomyChatHistory || [];
// Default: Suara MATI / Silent (Opsional via toggle)
window.isAudioSpeechEnabled = (localStorage.getItem('agri_chat_speech') === 'true');

window.renderAgronomyChatbot = function(containerId) {
  var container = document.getElementById(containerId);
  if (!container) return;

  var isSpeechOn = window.isAudioSpeechEnabled;
  var speechLabel = isSpeechOn ? "Suara Aktif" : "Suara Mati";
  var speechColor = isSpeechOn ? "#10b981" : "var(--text-sub)";

  container.innerHTML = `
    <div class="agri-chat-box">
      <div class="agri-chat-header">
        <div style="display:flex; align-items:center; gap:8px;">
          <div style="width:8px; height:8px; border-radius:50%; background:#a855f7; box-shadow:0 0 8px #a855f7;"></div>
          <span style="font-weight:700; font-size:12.5px; color:#c084fc;">Sobat Tani (Teman Curhat & Ngobrol Kebun)</span>
        </div>
        <div style="display:flex; align-items:center; gap:8px;">
          <button type="button" id="btn-toggle-speech" onclick="toggleChatSpeech()" style="background:rgba(255,255,255,0.05); border:1px solid rgba(255,255,255,0.1); border-radius:6px; color:${speechColor}; cursor:pointer; padding:3px 8px; font-size:11px; display:flex; align-items:center; gap:5px; transition:all 0.2s;" title="Aktifkan / Matikan Suara Pembacaan">
            <svg id="speech-icon" width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              ${isSpeechOn 
                ? '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/>'
                : '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><line x1="23" y1="9" x2="17" y2="15"/><line x1="17" y1="9" x2="23" y2="15"/>'}
            </svg>
            <span id="speech-status-label">${speechLabel}</span>
          </button>
        </div>
      </div>

      <div class="agri-chat-messages" id="agri-chat-messages">
        <div class="chat-bubble bot">
          Halo Mas/Pak! Ada yang lagi bikin bingung di kebun? Mau tanya racikan pupuk, obat semprot, atau curhat soal tanaman yang kurang seger, tanyain aja santai di sini ya!
        </div>
      </div>

      <!-- QUICK QUESTION PROMPT CHIPS -->
      <div class="chat-chips-row">
        <button type="button" class="chip-btn" onclick="askChatChip('Sebutkan contoh merek obat yang bagus di toko pertanian?')">
           Contoh Merek di Toko
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Bolehkah obat ini dicampur dengan pupuk daun atau perekat?')">
           Bisa Campur Pupuk Daun?
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Kapan waktu semprot yang paling tepat dan berapa hari sekali?')">
           Jam & Jadwal Semprot
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Bagaimana cara mencegah agar penyakit ini tidak menular ke bedengan lain?')">
           Cara Cegah Penularan
        </button>
      </div>

      <div class="agri-chat-input-bar">
        <input type="text" id="agri-chat-input" class="agri-chat-input" placeholder="Tanyakan apa saja seputar tanaman ini..." onkeydown="if(event.key==='Enter') submitAgronomyChat()">
        <button type="button" class="btn btn-primary" id="btn-send-agri-chat" onclick="submitAgronomyChat()" style="background:#a855f7; border:none; padding:0 14px; font-size:12px; font-weight:600; border-radius:8px; display:flex; align-items:center; justify-content:center;">
          <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="22" y1="2" x2="11" y2="13"/><polygon points="22 2 15 22 11 13 2 9 22 2"/></svg>
        </button>
      </div>
    </div>
  `;
};

window.toggleChatSpeech = function() {
  window.isAudioSpeechEnabled = !window.isAudioSpeechEnabled;
  localStorage.setItem('agri_chat_speech', window.isAudioSpeechEnabled ? 'true' : 'false');
  
  if (!window.isAudioSpeechEnabled && window.speechSynthesis) {
    window.speechSynthesis.cancel();
  }

  var label = document.getElementById('speech-status-label');
  var btn = document.getElementById('btn-toggle-speech');
  var icon = document.getElementById('speech-icon');
  
  if (label) {
    label.innerText = window.isAudioSpeechEnabled ? "Suara Aktif" : "Suara Mati";
  }
  if (btn) {
    btn.style.color = window.isAudioSpeechEnabled ? "#10b981" : "var(--text-sub)";
  }
  if (icon) {
    icon.innerHTML = window.isAudioSpeechEnabled
      ? '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/>'
      : '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><line x1="23" y1="9" x2="17" y2="15"/><line x1="17" y1="9" x2="23" y2="15"/>';
  }
};

window.askChatChip = function(questionText) {
  var input = document.getElementById('agri-chat-input');
  if (input) input.value = questionText;
  window.submitAgronomyChat();
};

window.submitAgronomyChat = async function() {
  var input = document.getElementById('agri-chat-input');
  var sendBtn = document.getElementById('btn-send-agri-chat');
  var msgBox = document.getElementById('agri-chat-messages');
  if (!input || !msgBox) return;

  var userText = input.value.trim();
  if (!userText) return;

  var apiKey = (window.getGeminiApiKey && window.getGeminiApiKey()) || localStorage.getItem("geminiApiKey") || window.DEFAULT_GEMINI_KEY;
  if (!apiKey || apiKey.length < 5) {
    msgBox.innerHTML += `
      <div class="chat-bubble bot" style="border-color:rgba(239,68,68,0.4); background:rgba(239,68,68,0.1);">
        <strong style="color:#f87171;">API Key Gemini Belum Terpasang</strong><br>
        Silakan masukkan API Key Gemini Anda di tab <strong>Agronomi & AI</strong>.
      </div>
    `;
    msgBox.scrollTop = msgBox.scrollHeight;
    return;
  }

  // Append user bubble
  msgBox.innerHTML += `
    <div class="chat-bubble user">${escapeHtmlText(userText)}</div>
  `;
  input.value = '';
  msgBox.scrollTop = msgBox.scrollHeight;

  // Typing indicator
  var typingId = 'typing-' + Date.now();
  msgBox.innerHTML += `
    <div class="chat-bubble bot" id="${typingId}" style="opacity:0.7; font-style:italic;">
      Sobat Tani sedang mikir jawaban...
    </div>
  `;
  msgBox.scrollTop = msgBox.scrollHeight;
  if (sendBtn) sendBtn.disabled = true;

  // Append to chat history with Sobat Tani Persona
  window.agronomyChatHistory = window.agronomyChatHistory || [];
  
  if (window.agronomyChatHistory.length === 0) {
    window.agronomyChatHistory.push({
      role: "user",
      parts: [{ text: `Instruksi Gaya Bicara Manusia (Wajib Patuh):
Kamu adalah kawan petani biasa yang sedang membalas chat WhatsApp singkat dari teman di kebun.
Aturan Wajib:
1. SUPER SINGKAT: Jawab HANYA dalam 1 sampai 3 kalimat pendek saja (maksimal 35–45 kata). Persis seperti pesan WhatsApp sesama kawan, bukan artikel atau rangkuman.
2. DILARANG BASA-BASI: Jangan pakai kalimat pembuka klise (jangan bilang "Wah bikin gemes ya", "Tenang saja mas", "Tentu saja", "Trik pakainya simpel kok", dsb). Langsung sebut inti jawaban dan solusinya!
3. Bahasa manusia santai: Santai, akrab, panggil "mas" atau "pak", langsung sebut merek yang terbukti dan takaran praktisnya.
Contoh gaya yang benar:
Tanya: "Sebutkan contoh nama merek obat fungisida bercak daun yang bagus?"
Jawab: "Cari Amistartop atau Score mas kalau udah parah. Tapi kalau baru gejala ringan, pake Antracol aja udah cukup, takarannya 2 sendok makan per tangki 16 liter."` }]
    }, {
      role: "model",
      parts: [{ text: "Siap, paham! Balasan saya selalu super singkat, langsung ke inti 1-2 kalimat kayak chat WA sesama kawan, tanpa basa-basi robot!" }]
    });
  }

  var tData = window.lastTelemetryData || {};
  var sensorContext = "";
  if (tData.temp !== undefined && tData.temp > 0) {
    sensorContext = ` (Lahan: Suhu ${tData.temp}°C, RH ${tData.hum}%, Tanah ${tData.soil}%)`;
  }

  window.agronomyChatHistory.push({
    role: "user",
    parts: [{ text: userText + sensorContext + "\n(PENTING: Balas super singkat 1-3 kalimat saja seperti chat WA kawan akrab. Langsung to-the-point tanpa kalimat pembuka klise!)" }]
  });

  try {
    const result = await window.fetchGeminiWithFallback(apiKey, {
      contents: window.agronomyChatHistory,
      generationConfig: {
        maxOutputTokens: 120,
        temperature: 0.65
      }
    });

    var botReply = result.candidates[0].content.parts[0].text;
    window.agronomyChatHistory.push({
      role: "model",
      parts: [{ text: botReply }]
    });

    var formattedReply = botReply
      .replace(/\*\*(.*?)\*\*/g, '<strong style="color:#e2e8f0;">$1</strong>')
      .replace(/\*(.*?)\*/g, '<em>$1</em>')
      .replace(/\n/g, '<br>');

    var typingElem = document.getElementById(typingId);
    if (typingElem) {
      typingElem.id = '';
      typingElem.style.opacity = '1';
      typingElem.style.fontStyle = 'normal';
      typingElem.innerHTML = formattedReply;
    }

    // Auto-scroll
    msgBox.scrollTop = msgBox.scrollHeight;

    // Speak aloud if enabled
    if (window.isAudioSpeechEnabled && window.speechSynthesis) {
      var plainSpeech = botReply.replace(/[*_#]/g, '').trim();
      var utterance = new SpeechSynthesisUtterance(plainSpeech);
      utterance.lang = 'id-ID';
      utterance.rate = 1.05;
      window.speechSynthesis.speak(utterance);
    }

    // Auto-save chat to localStorage & Firebase hook
    saveChatToStorage(userText, botReply);

  } catch (err) {
    var typingElem = document.getElementById(typingId);
    if (typingElem) {
      typingElem.innerHTML = `<span style="color:#f87171;">Gagal mendapatkan jawaban: ${err.message}</span>`;
    }
  } finally {
    if (sendBtn) sendBtn.disabled = false;
  }
}

function escapeHtmlText(str) {
  return str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

function saveChatToStorage(userMsg, botMsg) {
  try {
    var saved = JSON.parse(localStorage.getItem("smartfarm_agri_chat") || "[]");
    var chatRecord = {
      timestamp: new Date().toISOString(),
      user: userMsg,
      bot: botMsg,
      telemetry: window.lastTelemetryData || {}
    };
    saved.push(chatRecord);
    if (saved.length > 30) saved.shift(); // Keep last 30 messages
    localStorage.setItem("smartfarm_agri_chat", JSON.stringify(saved));

    // Hook to sync to Firebase if configured
    if (window.syncChatToFirebase) {
      window.syncChatToFirebase(chatRecord);
    }
  } catch (e) {
    console.warn("Could not save chat history:", e);
  }
}

// Ready-to-use Firebase Realtime Database / Firestore sync hook
window.syncChatToFirebase = function(chatRecord) {
  var firebaseDbUrl = localStorage.getItem("firebaseDatabaseUrl");
  if (firebaseDbUrl && firebaseDbUrl.startsWith("http")) {
    var endpoint = firebaseDbUrl.replace(/\/$/, "") + "/agronomy_chats.json";
    fetch(endpoint, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(chatRecord)
    }).catch(function(err) {
      console.warn("Firebase sync failed:", err);
    });
  }
};
