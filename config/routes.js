const axios = require("axios")
const bcrypt = require("bcryptjs")
const jwt = require("jsonwebtoken")
const db = require("../database/dbConfig")
const secret = process.env.JWT_SECRET

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
    .then(([id]) => {
      res.status(201).json({
        username,
        password: hash,
        id
      })
    })
    .catch(error => {
      res.status(500).json(error)
    })
}

function login(req, res) {
  const { username, password } = req.body

  if (!username || !password) {
    res.status(400).json({
      message: "Please provide a username and password."
    })
  }

  db("users")
    .where({ username })
    .first()
    .then(user => {
      if (user && bcrypt.compareSync(password, user.password)) {
        const token = genToken(user)
        res.status(200).json({
          message: `Welcome ${user.username}!`,
          token
        })
      } else {
        res.status(401).json({ message: "Invalid Credentials" })
      }
    })
    .catch(error => {
      res.status(500).json(error)
    })
}

function getJokes(_req, res) {
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

function genToken(user) {
  const payload = {
    subject: user.id,
    username: user.username
  }
  const options = {
    expiresIn: "1d"
  }
  return jwt.sign(payload, secret, options)
}
