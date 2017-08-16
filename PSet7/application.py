from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_requestg 
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    totalMoney = 0

    # Get portfolio (all stocks user have)
    rows = db.execute("SELECT * FROM (SELECT portfolio.*, symbols.name FROM portfolio JOIN symbols ON portfolio.stockSymbol = symbols.symbol) WHERE user_id = :id",
    id = session["user_id"])
    for row in rows:
        price = float(lookup(row["stockSymbol"])["price"])
        row["price"] = usd(price)
        row["total"] = usd(price * int(row["ammount"]))
        # Compute money in stocks
        totalMoney += price * int(row["ammount"])

    # Get cash
    cash = float(db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])[0]["cash"])
    totalMoney += cash
    return render_template("index.html", cash = usd(cash), totalMoney = usd(totalMoney), stocks = rows)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        stockSymbol = request.form.get("stockSymbol").upper()
        stock = lookup(stockSymbol)
        if not stock:
            return apology("{} not found".format(stockSymbol))
        if not request.form.get("ammount"):
            return apology("ammount cannot be null")
        ammount = float(request.form.get("ammount"))
        if ammount <= 0:
            return apology("ammount must be greater than 0")
        id = session["user_id"]
        row = db.execute("SELECT * FROM symbols WHERE symbol = :symbol", symbol = stockSymbol)
        if not row:
            db.execute("INSERT INTO symbols (symbol, name) VALUES (:symbol, :name)",
            symbol = stockSymbol, name = stock["name"])
        row = db.execute("SELECT * FROM users WHERE id = :id",
        id = id)

        cash = float(row[0]["cash"])
        cash -= float(stock["price"]) * ammount
        if cash < 0:
            return apology("don't have so much cash")

        row = db.execute("SELECT * FROM portfolio WHERE user_id = :id AND stockSymbol = :stockSymbol", id = id, stockSymbol = stockSymbol)
        if not row:
            db.execute("INSERT INTO portfolio (user_id, stockSymbol, ammount) VALUES (:user_id, :stockSymbol, :ammount)", user_id = id, stockSymbol = stockSymbol, ammount = ammount)
        else:
            db.execute("UPDATE portfolio SET ammount = :ammount WHERE user_id = :user_id AND stockSymbol = :stockSymbol", ammount = ammount + int(row[0]["ammount"]), user_id = id, stockSymbol = stockSymbol)
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id = id)
        db.execute("INSERT INTO history (user_id, stockSymbol, price, ammount, balanceAfter, date) VALUES (:user_id, :stockSymbol, :price, :ammount, :balanceAfter, datetime())", 
        user_id = id, stockSymbol = stockSymbol, price = float(stock["price"]), ammount= ammount, 
        balanceAfter = cash)
        flash("You succesfully bought {} {} stock".format(int(ammount), stockSymbol))
        return redirect(url_for("index"))
    rows = db.execute("SELECT stockSymbol FROM portfolio WHERE user_id = :id", id = session["user_id"])
    return render_template("buy.html", portfolio=rows)

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM (SELECT history.*, symbols.name FROM history JOIN symbols ON history.stockSymbol = symbols.symbol) WHERE user_id = :id", id = session["user_id"])
    for row in rows:
        row["price"] = usd(row["price"])
        row["balanceAfter"] = usd(row["balanceAfter"])
    
    rows = rows[::-1]
    return render_template("history.html", transactions = rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        stocks = lookup(request.form.get("stockSymbol").upper())
        if not stocks:
            return apology("{} not found".format(request.form.get("stockSymbol")))
        row = db.execute("SELECT * FROM symbols WHERE symbol = :symbol", symbol = request.form.get("stockSymbol").upper())
        if not row:
            db.execute("INSERT INTO symbols (symbol, name) VALUES (:symbol, :name)", symbol = request.form.get("stockSymbol").upper(),
            name = stocks["name"])
        stocks["price"] = usd(stocks["price"])
        return render_template("quoted.html", stocks=stocks)

    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username")
        if not request.form.get("password") or not request.form.get("passwordValidate"):
            return apology("must provide password")
        if request.form.get("password") != request.form.get("passwordValidate"):
            return apology("password wasn't same")
        
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) != 0:
            return apology("username taken")
        
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", 
        username = request.form.get("username"),
        hash = pwd_context.hash(request.form.get("password")))

        flash("You succesfully register! Now you can login!")
        return redirect(url_for("login"))

    return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        stock = lookup(request.form.get("stockSymbol"))
        if not stock:
            return apology("cannot found that stock")
        ammount = int(request.form.get("ammount"))
        print("DEBUG!!!!!! {}".format(ammount))
        if ammount <= 0:
            return apology("Amount must be greater than 0")
        row = db.execute("SELECT ammount FROM portfolio WHERE user_id = :id", id = session["user_id"])
        if not row:
            return apology("You do not have that")

        howMuchHave = int(row[0]["ammount"])
        print("DEBUG!!!!!!!!!!!!!!! " + request.form.get("stockSymbol"))
        if ammount > howMuchHave:
            return apology("You do not have that much stock")

        if howMuchHave == ammount:
            db.execute("DELETE FROM portfolio WHERE user_id = :id AND stockSymbol = :stock", id = session["user_id"], stock = request.form.get("stockSymbol"))
        else:
            print("JESTEM TUTAJ!")
            db.execute("UPDATE portfolio SET ammount = :ammount WHERE user_id = :id AND stockSymbol = :stock", ammount = howMuchHave - ammount, id = session["user_id"], stock = request.form.get("stockSymbol"))
        
        cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"])
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash + ammount * float(stock["price"]), id = session["user_id"])
        
        db.execute("INSERT INTO history (user_id, stockSymbol, price, ammount, balanceAfter, date) VALUES (:user_id, :stockSymbol, :price, :ammount, :balanceAfter, datetime())", 
        user_id = session["user_id"], stockSymbol = request.form.get("stockSymbol"), price = ammount * float(stock["price"]), ammount = -ammount, balanceAfter = cash + ammount * float(stock["price"]))
        
        flash("Succesfully sold {} for {} giving total cash {}".format(request.form.get("stockSymbol"), usd(float(stock["price"])), usd(ammount * float(stock["price"]))))
        return redirect(url_for("index"))
    rows = db.execute("SELECT stockSymbol FROM portfolio WHERE user_id = :id", id = session["user_id"])

    return render_template("sell.html", availableStocks = rows)
	
if __name__ == "__main__":
	app.run()
