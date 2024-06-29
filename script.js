document.addEventListener("DOMContentLoaded", () => {
    const diceElements = [
        document.getElementById("die1"),
        document.getElementById("die2"),
        document.getElementById("die3"),
        document.getElementById("die4"),
        document.getElementById("die5")
    ];
    const rollButton = document.getElementById("rollButton");
    const rerollButton = document.getElementById("rerollButton");
    const categoryElements = document.querySelectorAll(".category");
    const totalScoreElement = document.getElementById("total-score");

    let dice = [1, 2, 3, 4, 5];
    let scores = {
        "ones": null,
        "twos": null,
        "threes": null,
        "fours": null,
        "fives": null,
        "sixes": null,
        "three of a kind": null,
        "four of a kind": null,
        "full house": null,
        "small straight": null,
        "large straight": null,
        "yahtzee": null,
        "chance": null
    };

    // Prevent default scrolling behavior affecting dice values
    document.addEventListener("wheel", (event) => {
        event.preventDefault();
    }, { passive: false });

    rollButton.addEventListener("click", rollDice);
    rerollButton.addEventListener("click", rerollDice);
    categoryElements.forEach(category => category.addEventListener("click", handleCategoryClick));

    function rollDice() {
        dice = dice.map(() => Math.floor(Math.random() * 6) + 1);
        updateDiceDisplay();
        updateScores();
        rerollButton.disabled = false;
    }

    function rerollDice() {
        dice = dice.map((die, index) => diceElements[index].classList.contains("selected") ? Math.floor(Math.random() * 6) + 1 : die);
        updateDiceDisplay();
        updateScores();
    }

    function updateDiceDisplay() {
        diceElements.forEach((dieElement, index) => {
            dieElement.textContent = dice[index];
            dieElement.classList.remove("selected");
        });
    }

    function updateScores() {
        categoryElements.forEach(categoryElement => {
            const category = categoryElement.dataset.category;
            if (scores[category] === null) {
                const score = calculateScore(dice, category);
                scores[category] = score;
                document.getElementById(`score-${category.replace(/ /g, '-')}`).textContent = score;
                updateTotalScore();
            }
        });
    }

    function calculateScore(dice, category) {
        const counts = dice.reduce((acc, die) => {
            acc[die] = (acc[die] || 0) + 1;
            return acc;
        }, {});

        switch (category) {
            case "ones":
                return (counts[1] || 0) * 1;
            case "twos":
                return (counts[2] || 0) * 2;
            case "threes":
                return (counts[3] || 0) * 3;
            case "fours":
                return (counts[4] || 0) * 4;
            case "fives":
                return (counts[5] || 0) * 5;
            case "sixes":
                return (counts[6] || 0) * 6;
            case "three of a kind":
                return Object.values(counts).some(count => count >= 3) ? dice.reduce((a, b) => a + b, 0) : 0;
            case "four of a kind":
                return Object.values(counts).some(count => count >= 4) ? dice.reduce((a, b) => a + b, 0) : 0;
            case "full house":
                return Object.values(counts).includes(3) && Object.values(counts).includes(2) ? 25 : 0;
            case "small straight":
                return [1, 2, 3, 4].every(n => counts[n]) || [2, 3, 4, 5].every(n => counts[n]) || [3, 4, 5, 6].every(n => counts[n]) ? 30 : 0;
            case "large straight":
                return [1, 2, 3, 4, 5].every(n => counts[n]) || [2, 3, 4, 5, 6].every(n => counts[n]) ? 40 : 0;
            case "yahtzee":
                return Object.values(counts).some(count => count === 5) ? 50 : 0;
            case "chance":
                return dice.reduce((a, b) => a + b, 0);
            default:
                return 0;
        }
    }

    function updateTotalScore() {
        const totalScore = Object.values(scores).reduce((total, score) => total + (score || 0), 0);
        totalScoreElement.textContent = totalScore;
    }

    function handleCategoryClick(event) {
        const category = event.currentTarget.dataset.category;
        if (scores[category] === null) {
            const score = calculateScore(dice, category);
            scores[category] = score;
            document.getElementById(`score-${category.replace(/ /g, '-')}`).textContent = score;
            updateTotalScore();
        }
    }
});
