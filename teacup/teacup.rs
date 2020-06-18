use std::collections::BTreeSet;
use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};

fn load_dictionary(filename : &str) -> std::io::Result<BTreeSet<String>> {
    let file = File::open(filename)?;
    let mut dict = BTreeSet::new();
    for line in io::BufReader::new(file).lines() {
        if let Ok(word) = line {
            dict.insert(word);
        }
    }
    Ok(dict)
}

fn find_teacup_words(dict : &BTreeSet<String>) {
    let mut teacup_words : BTreeSet<String> = BTreeSet::new();
    let mut found : HashSet<String> = HashSet::new();
    for word in dict {
        let len = word.len();
        if len < 3 || found.contains(word) {
            continue;
        }
        teacup_words.clear();
        let mut is_teacup_word = true;
        let mut bytes = word.as_bytes().to_vec();
        for _ in 1..len {
            bytes.rotate_left(1);
            match String::from_utf8(bytes.to_vec()) {
                Ok(w) => {
                    if !dict.contains(&w) {
                        is_teacup_word = false;
                        break;
                    }
                    if !w.eq(word) {
                        teacup_words.insert(w);
                    }
                },
                Err(_) => {
                    is_teacup_word = false;
                    break;
                }
            }
        }
        if !is_teacup_word || teacup_words.is_empty() {
            continue;
        }
        print!("{}", word);
        found.insert(word.to_string());
        for w in &teacup_words {
            found.insert(w.to_string());
            print!(" {}", w);
        }
        println!();
    }
}

fn main() {
    let args : Vec<String> = std::env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: teacup dictionary");
        std::process::exit(1);
    }
    let dict = load_dictionary(&args[1]);
    match dict {
        Ok(dict) => find_teacup_words(&dict),
        Err(error) => eprintln!("Cannot open file {}: {}", &args[1], error)
    }
}
