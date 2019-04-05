const axios = require("axios")
const bcrypt = require("bcryptjs")
const jwt = require("jsonwebtoken")
const db = require("../database/dbConfig")

const { authenticate } = require("../auth/authenticate")

module.exports = server => {
  server.post("/api/register", register)
  server.post("/api/login", login)
  server.get("/api/jokes", authenticate, getJokes)
}

function register(req, res) {
  const { username, password } = req.body

  if (!username || !password) {
    res.status(400).json({
      message: "Please provide a username and password."
    })
  }

  const hash = bcrypt.hashSync(password, 10)

  db("users")
    .insert({ username, password: hash })
    .then(saved => {
      res.status(201).json(saved)
    })
    .catch(error => {
      res.status(500).json(error)
    })
}

function login(req, res) {
  // implement user login
}

function getJokes(req, res) {
  const requestOptions = {
    headers: { accept: "application/json" }
  }

  axios
    .get("https://icanhazdadjoke.com/search", requestOptions)
    .then(response => {
      res.status(200).json(response.data.results)
    })
    .catch(err => {
      res.status(500).json({ message: "Error Fetching Jokes", error: err })
    })
}
