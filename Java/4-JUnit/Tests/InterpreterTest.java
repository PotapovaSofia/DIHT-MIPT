package ru.fizteh.fivt.students.PotapovaSofia.JUnit.Tests;

import static org.junit.Assert.*;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.function.BiConsumer;

import org.junit.Before;
import org.junit.Test;
import ru.fizteh.fivt.students.PotapovaSofia.JUnit.Interpreter.Command;
import ru.fizteh.fivt.students.PotapovaSofia.JUnit.Interpreter.Interpreter;
import ru.fizteh.fivt.students.PotapovaSofia.JUnit.TableState;

public class InterpreterTest {
    private final String newLine = System.getProperty("line.separator");
    private final String testCommand = "test";
    private final String testOutput = "TEST";
    private ByteArrayOutputStream outputStream;
    private PrintStream printStream;

    @Before
    public void setUp() {
        outputStream = new ByteArrayOutputStream();
        printStream = new PrintStream(outputStream);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInterpreterThrowsExceptionConstructedForNullStream() {
        new Interpreter(null, new Command[] {}, null, null);
    }

    @Test
    public void testInteractiveMode() throws Exception {
        Interpreter interpreter = new Interpreter(null, new Command[] {
                new Command(testCommand, 0, new BiConsumer<TableState, String[]>() {
                    @Override
                    public void accept(TableState tableState, String[] arguments) {
                        printStream.println(testOutput);
                    }
                })}, new ByteArrayInputStream(
                (testCommand + newLine + "exit" + newLine).getBytes()), printStream);
        interpreter.run(new String[] {});
        assertEquals(Interpreter.PROMPT + testOutput + newLine + Interpreter.PROMPT,
                outputStream.toString());
    }

    @Test
    public void testBatchMode() throws Exception {
        Interpreter interpreter = new Interpreter(null, new Command[] {
                new Command(testCommand, 0, new BiConsumer<TableState, String[]>() {
                    @Override
                    public void accept(TableState tableState, String[] arguments) {
                        printStream.println(testOutput);
                    }
                })}, new ByteArrayInputStream(new byte[] {}), printStream);
        interpreter.run(new String[] {testCommand + Interpreter.STATEMENT_DELIMITER, testCommand});
        assertEquals(testOutput + newLine + testOutput + newLine, outputStream.toString());
    }

    @Test
    public void testBatchModeForUnexpectedCommand() throws Exception {
        Interpreter interpreter = new Interpreter(null, new Command[] {},
                new ByteArrayInputStream(new byte[] {}), printStream);
        interpreter.run(new String[] {testCommand});
        assertEquals(Interpreter.COMMAND_NOT_FOUND_MSG + testCommand + newLine, outputStream.toString());
    }

    @Test
    public void testInteractiveModeForUnexpectedCommand() throws Exception {
        Interpreter interpreter = new Interpreter(null, new Command[]{}, new ByteArrayInputStream(("wrongCommand"
                + newLine + "exit" + newLine).getBytes()), printStream);
        interpreter.run(new String[]{});
        String actual = outputStream.toString();
        String expected = new String(Interpreter.PROMPT + Interpreter.COMMAND_NOT_FOUND_MSG + "wrongCommand" + newLine
                + Interpreter.PROMPT);
        assertEquals(actual, expected);
    }

    @Test
    public void testOnCommandWithWrongNumberOfArguments() throws Exception {
        Interpreter interpreter = new Interpreter(null, new Command[] {
                new Command(testCommand, 0, new BiConsumer<TableState, String[]>() {
                    @Override
                    public void accept(TableState tableState, String[] arguments) {
                        printStream.println(testOutput);
                    }
                })}, new ByteArrayInputStream(new byte[] {}), printStream);
        interpreter.run(new String[] {testCommand + " some_argument"});
    }
}
